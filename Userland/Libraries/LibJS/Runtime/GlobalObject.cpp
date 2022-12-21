/*
 * Copyright (c) 2020, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2020-2023, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/BuiltinWrappers.h>
#include <AK/CharacterTypes.h>
#include <AK/FloatingPointStringConversions.h>
#include <AK/Hex.h>
#include <AK/UnicodeUtils.h>
#include <AK/Utf16View.h>
#include <AK/Utf8View.h>
#include <LibJS/Heap/DeferGC.h>
#include <LibJS/Interpreter.h>
#include <LibJS/Runtime/AbstractOperations.h>
#include <LibJS/Runtime/AggregateErrorConstructor.h>
#include <LibJS/Runtime/ArrayBufferConstructor.h>
#include <LibJS/Runtime/ArrayConstructor.h>
#include <LibJS/Runtime/ArrayPrototype.h>
#include <LibJS/Runtime/AsyncFunctionConstructor.h>
#include <LibJS/Runtime/AsyncGeneratorFunctionConstructor.h>
#include <LibJS/Runtime/AsyncGeneratorPrototype.h>
#include <LibJS/Runtime/AtomicsObject.h>
#include <LibJS/Runtime/BigIntConstructor.h>
#include <LibJS/Runtime/BooleanConstructor.h>
#include <LibJS/Runtime/ConsoleObject.h>
#include <LibJS/Runtime/DataViewConstructor.h>
#include <LibJS/Runtime/DateConstructor.h>
#include <LibJS/Runtime/DisposableStackConstructor.h>
#include <LibJS/Runtime/ErrorConstructor.h>
#include <LibJS/Runtime/FinalizationRegistryConstructor.h>
#include <LibJS/Runtime/FinalizationRegistryPrototype.h>
#include <LibJS/Runtime/FunctionConstructor.h>
#include <LibJS/Runtime/GeneratorFunctionConstructor.h>
#include <LibJS/Runtime/GeneratorPrototype.h>
#include <LibJS/Runtime/GlobalEnvironment.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Intl/CollatorConstructor.h>
#include <LibJS/Runtime/Intl/DateTimeFormatConstructor.h>
#include <LibJS/Runtime/Intl/DisplayNamesConstructor.h>
#include <LibJS/Runtime/Intl/DurationFormatConstructor.h>
#include <LibJS/Runtime/Intl/Intl.h>
#include <LibJS/Runtime/Intl/ListFormatConstructor.h>
#include <LibJS/Runtime/Intl/LocaleConstructor.h>
#include <LibJS/Runtime/Intl/NumberFormatConstructor.h>
#include <LibJS/Runtime/Intl/PluralRulesConstructor.h>
#include <LibJS/Runtime/Intl/RelativeTimeFormatConstructor.h>
#include <LibJS/Runtime/Intl/SegmenterConstructor.h>
#include <LibJS/Runtime/JSONObject.h>
#include <LibJS/Runtime/MapConstructor.h>
#include <LibJS/Runtime/MathObject.h>
#include <LibJS/Runtime/NumberConstructor.h>
#include <LibJS/Runtime/Object.h>
#include <LibJS/Runtime/ObjectConstructor.h>
#include <LibJS/Runtime/PromiseConstructor.h>
#include <LibJS/Runtime/ProxyConstructor.h>
#include <LibJS/Runtime/Realm.h>
#include <LibJS/Runtime/ReflectObject.h>
#include <LibJS/Runtime/RegExpConstructor.h>
#include <LibJS/Runtime/SetConstructor.h>
#include <LibJS/Runtime/ShadowRealmConstructor.h>
#include <LibJS/Runtime/Shape.h>
#include <LibJS/Runtime/StringConstructor.h>
#include <LibJS/Runtime/StringPrototype.h>
#include <LibJS/Runtime/SuppressedErrorConstructor.h>
#include <LibJS/Runtime/SymbolConstructor.h>
#include <LibJS/Runtime/Temporal/CalendarConstructor.h>
#include <LibJS/Runtime/Temporal/DurationConstructor.h>
#include <LibJS/Runtime/Temporal/InstantConstructor.h>
#include <LibJS/Runtime/Temporal/PlainDateConstructor.h>
#include <LibJS/Runtime/Temporal/PlainDateTimeConstructor.h>
#include <LibJS/Runtime/Temporal/PlainMonthDayConstructor.h>
#include <LibJS/Runtime/Temporal/PlainTimeConstructor.h>
#include <LibJS/Runtime/Temporal/PlainYearMonthConstructor.h>
#include <LibJS/Runtime/Temporal/Temporal.h>
#include <LibJS/Runtime/Temporal/TimeZoneConstructor.h>
#include <LibJS/Runtime/Temporal/ZonedDateTimeConstructor.h>
#include <LibJS/Runtime/TypedArray.h>
#include <LibJS/Runtime/Value.h>
#include <LibJS/Runtime/WeakMapConstructor.h>
#include <LibJS/Runtime/WeakRefConstructor.h>
#include <LibJS/Runtime/WeakSetConstructor.h>

namespace JS {

GlobalObject::GlobalObject(Realm& realm)
    : Object(GlobalObjectTag::Tag, realm)
{
    ensure_shape_is_unique();
    Object::set_prototype(realm.intrinsics().object_prototype());
}

// 9.3.4 SetDefaultGlobalBindings ( realmRec ), https://tc39.es/ecma262/#sec-setdefaultglobalbindings
Object& set_default_global_bindings(Realm& realm)
{
    auto& vm = realm.vm();

    // 1. Let global be realmRec.[[GlobalObject]].
    auto& global = realm.global_object();

    // 2. For each property of the Global Object specified in clause 19, do
    //     a. Let name be the String value of the property name.
    //     b. Let desc be the fully populated data Property Descriptor for the property, containing the specified attributes for the property.
    //        For properties listed in 19.2, 19.3, or 19.4 the value of the [[Value]] attribute is the corresponding intrinsic object from realmRec.
    //     c. Perform ? DefinePropertyOrThrow(global, name, desc).
    //     NOTE: This function is infallible as we set properties directly; property clashes in global object construction are not expected.

    u8 attr = Attribute::Writable | Attribute::Configurable;

    // 19.2 Function Properties of the Global Object, https://tc39.es/ecma262/#sec-function-properties-of-the-global-object
    global.define_direct_property(vm.names.eval, realm.intrinsics().eval_function(), attr);
    global.define_direct_property(vm.names.isFinite, realm.intrinsics().is_finite_function(), attr);
    global.define_direct_property(vm.names.isNaN, realm.intrinsics().is_nan_function(), attr);
    global.define_direct_property(vm.names.parseFloat, realm.intrinsics().parse_float_function(), attr);
    global.define_direct_property(vm.names.parseInt, realm.intrinsics().parse_int_function(), attr);
    global.define_direct_property(vm.names.decodeURI, realm.intrinsics().decode_uri_function(), attr);
    global.define_direct_property(vm.names.decodeURIComponent, realm.intrinsics().decode_uri_component_function(), attr);
    global.define_direct_property(vm.names.encodeURI, realm.intrinsics().encode_uri_function(), attr);
    global.define_direct_property(vm.names.encodeURIComponent, realm.intrinsics().encode_uri_component_function(), attr);

    // 19.1 Value Properties of the Global Object, https://tc39.es/ecma262/#sec-value-properties-of-the-global-object
    global.define_direct_property(vm.names.globalThis, &global, attr);
    global.define_direct_property(vm.names.Infinity, js_infinity(), 0);
    global.define_direct_property(vm.names.NaN, js_nan(), 0);
    global.define_direct_property(vm.names.undefined, js_undefined(), 0);

    // 19.3 Constructor Properties of the Global Object, https://tc39.es/ecma262/#sec-constructor-properties-of-the-global-object
    global.define_intrinsic_accessor(vm.names.AggregateError, attr, [](auto& realm) -> Value { return realm.intrinsics().aggregate_error_constructor(); });
    global.define_intrinsic_accessor(vm.names.Array, attr, [](auto& realm) -> Value { return realm.intrinsics().array_constructor(); });
    global.define_intrinsic_accessor(vm.names.ArrayBuffer, attr, [](auto& realm) -> Value { return realm.intrinsics().array_buffer_constructor(); });
    global.define_intrinsic_accessor(vm.names.BigInt, attr, [](auto& realm) -> Value { return realm.intrinsics().bigint_constructor(); });
    global.define_intrinsic_accessor(vm.names.BigInt64Array, attr, [](auto& realm) -> Value { return realm.intrinsics().big_int64_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.BigUint64Array, attr, [](auto& realm) -> Value { return realm.intrinsics().big_uint64_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.Boolean, attr, [](auto& realm) -> Value { return realm.intrinsics().boolean_constructor(); });
    global.define_intrinsic_accessor(vm.names.DataView, attr, [](auto& realm) -> Value { return realm.intrinsics().data_view_constructor(); });
    global.define_intrinsic_accessor(vm.names.Date, attr, [](auto& realm) -> Value { return realm.intrinsics().date_constructor(); });
    global.define_intrinsic_accessor(vm.names.DisposableStack, attr, [](auto& realm) -> Value { return realm.intrinsics().disposable_stack_constructor(); });
    global.define_intrinsic_accessor(vm.names.Error, attr, [](auto& realm) -> Value { return realm.intrinsics().error_constructor(); });
    global.define_intrinsic_accessor(vm.names.EvalError, attr, [](auto& realm) -> Value { return realm.intrinsics().eval_error_constructor(); });
    global.define_intrinsic_accessor(vm.names.FinalizationRegistry, attr, [](auto& realm) -> Value { return realm.intrinsics().finalization_registry_constructor(); });
    global.define_intrinsic_accessor(vm.names.Float32Array, attr, [](auto& realm) -> Value { return realm.intrinsics().float32_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.Float64Array, attr, [](auto& realm) -> Value { return realm.intrinsics().float64_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.Function, attr, [](auto& realm) -> Value { return realm.intrinsics().function_constructor(); });
    global.define_intrinsic_accessor(vm.names.Int8Array, attr, [](auto& realm) -> Value { return realm.intrinsics().int8_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.Int16Array, attr, [](auto& realm) -> Value { return realm.intrinsics().int16_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.Int32Array, attr, [](auto& realm) -> Value { return realm.intrinsics().int32_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.Map, attr, [](auto& realm) -> Value { return realm.intrinsics().map_constructor(); });
    global.define_intrinsic_accessor(vm.names.Number, attr, [](auto& realm) -> Value { return realm.intrinsics().number_constructor(); });
    global.define_intrinsic_accessor(vm.names.Object, attr, [](auto& realm) -> Value { return realm.intrinsics().object_constructor(); });
    global.define_intrinsic_accessor(vm.names.Promise, attr, [](auto& realm) -> Value { return realm.intrinsics().promise_constructor(); });
    global.define_intrinsic_accessor(vm.names.Proxy, attr, [](auto& realm) -> Value { return realm.intrinsics().proxy_constructor(); });
    global.define_intrinsic_accessor(vm.names.RangeError, attr, [](auto& realm) -> Value { return realm.intrinsics().range_error_constructor(); });
    global.define_intrinsic_accessor(vm.names.ReferenceError, attr, [](auto& realm) -> Value { return realm.intrinsics().reference_error_constructor(); });
    global.define_intrinsic_accessor(vm.names.RegExp, attr, [](auto& realm) -> Value { return realm.intrinsics().regexp_constructor(); });
    global.define_intrinsic_accessor(vm.names.Set, attr, [](auto& realm) -> Value { return realm.intrinsics().set_constructor(); });
    global.define_intrinsic_accessor(vm.names.ShadowRealm, attr, [](auto& realm) -> Value { return realm.intrinsics().shadow_realm_constructor(); });
    global.define_intrinsic_accessor(vm.names.String, attr, [](auto& realm) -> Value { return realm.intrinsics().string_constructor(); });
    global.define_intrinsic_accessor(vm.names.SuppressedError, attr, [](auto& realm) -> Value { return realm.intrinsics().suppressed_error_constructor(); });
    global.define_intrinsic_accessor(vm.names.Symbol, attr, [](auto& realm) -> Value { return realm.intrinsics().symbol_constructor(); });
    global.define_intrinsic_accessor(vm.names.SyntaxError, attr, [](auto& realm) -> Value { return realm.intrinsics().syntax_error_constructor(); });
    global.define_intrinsic_accessor(vm.names.TypeError, attr, [](auto& realm) -> Value { return realm.intrinsics().type_error_constructor(); });
    global.define_intrinsic_accessor(vm.names.Uint8Array, attr, [](auto& realm) -> Value { return realm.intrinsics().uint8_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.Uint8ClampedArray, attr, [](auto& realm) -> Value { return realm.intrinsics().uint8_clamped_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.Uint16Array, attr, [](auto& realm) -> Value { return realm.intrinsics().uint16_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.Uint32Array, attr, [](auto& realm) -> Value { return realm.intrinsics().uint32_array_constructor(); });
    global.define_intrinsic_accessor(vm.names.URIError, attr, [](auto& realm) -> Value { return realm.intrinsics().uri_error_constructor(); });
    global.define_intrinsic_accessor(vm.names.WeakMap, attr, [](auto& realm) -> Value { return realm.intrinsics().weak_map_constructor(); });
    global.define_intrinsic_accessor(vm.names.WeakRef, attr, [](auto& realm) -> Value { return realm.intrinsics().weak_ref_constructor(); });
    global.define_intrinsic_accessor(vm.names.WeakSet, attr, [](auto& realm) -> Value { return realm.intrinsics().weak_set_constructor(); });

    // 19.4 Other Properties of the Global Object, https://tc39.es/ecma262/#sec-other-properties-of-the-global-object
    global.define_intrinsic_accessor(vm.names.Atomics, attr, [](auto& realm) -> Value { return realm.intrinsics().atomics_object(); });
    global.define_intrinsic_accessor(vm.names.Intl, attr, [](auto& realm) -> Value { return realm.intrinsics().intl_object(); });
    global.define_intrinsic_accessor(vm.names.JSON, attr, [](auto& realm) -> Value { return realm.intrinsics().json_object(); });
    global.define_intrinsic_accessor(vm.names.Math, attr, [](auto& realm) -> Value { return realm.intrinsics().math_object(); });
    global.define_intrinsic_accessor(vm.names.Reflect, attr, [](auto& realm) -> Value { return realm.intrinsics().reflect_object(); });
    global.define_intrinsic_accessor(vm.names.Temporal, attr, [](auto& realm) -> Value { return realm.intrinsics().temporal_object(); });

    // B.2.1 Additional Properties of the Global Object, https://tc39.es/ecma262/#sec-additional-properties-of-the-global-object
    global.define_direct_property(vm.names.escape, realm.intrinsics().escape_function(), attr);
    global.define_direct_property(vm.names.unescape, realm.intrinsics().unescape_function(), attr);

    // Non-standard
    global.define_direct_property(vm.names.InternalError, realm.intrinsics().internal_error_constructor(), attr);
    global.define_direct_property(vm.names.console, realm.intrinsics().console_object(), attr);

    // 3. Return global.
    return global;
}

void GlobalObject::initialize(Realm& realm)
{
    Base::initialize(realm);

    auto& vm = this->vm();

    // Non-standard
    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(realm, vm.names.gc, gc, 0, attr);
}

GlobalObject::~GlobalObject() = default;

JS_DEFINE_NATIVE_FUNCTION(GlobalObject::gc)
{
#ifdef AK_OS_SERENITY
    dbgln("Forced garbage collection requested!");
#endif
    vm.heap().collect_garbage();
    return js_undefined();
}

// 19.2.3 isNaN ( number ), https://tc39.es/ecma262/#sec-isnan-number
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::is_nan)
{
    return Value(TRY(vm.argument(0).to_number(vm)).is_nan());
}

// 19.2.2 isFinite ( number ), https://tc39.es/ecma262/#sec-isfinite-number
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::is_finite)
{
    return Value(TRY(vm.argument(0).to_number(vm)).is_finite_number());
}

// 19.2.4 parseFloat ( string ), https://tc39.es/ecma262/#sec-parsefloat-string
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::parse_float)
{
    // OPTIMIZATION: We can skip the number-to-string-to-number round trip when the value is already a number.
    if (vm.argument(0).is_number())
        return vm.argument(0);

    // 1. Let inputString be ? ToString(string).
    auto input_string = TRY(vm.argument(0).to_deprecated_string(vm));

    // 2. Let trimmedString be ! TrimString(inputString, start).
    auto trimmed_string = MUST_OR_THROW_OOM(trim_string(vm, PrimitiveString::create(vm, input_string), TrimMode::Left));
    if (trimmed_string.is_empty())
        return js_nan();

    // 3. If neither trimmedString nor any prefix of trimmedString satisfies the syntax of a StrDecimalLiteral (see 7.1.4.1), return NaN.
    // 4. Let numberString be the longest prefix of trimmedString, which might be trimmedString itself, that satisfies the syntax of a StrDecimalLiteral.
    // 5. Let parsedNumber be ParseText(StringToCodePoints(numberString), StrDecimalLiteral).
    // 6. Assert: parsedNumber is a Parse Node.
    // 7. Return StringNumericValue of parsedNumber.
    auto trimmed_string_view = trimmed_string.bytes_as_string_view();
    auto const* begin = trimmed_string_view.characters_without_null_termination();
    auto const* end = begin + trimmed_string_view.length();

    auto parsed_number = parse_first_floating_point<double>(begin, end);
    if (parsed_number.parsed_value())
        return parsed_number.value;

    auto first_code_point = *trimmed_string.code_points().begin();
    if (first_code_point == '-' || first_code_point == '+')
        trimmed_string_view = trimmed_string_view.substring_view(1);

    if (trimmed_string_view.starts_with("Infinity"sv, AK::CaseSensitivity::CaseSensitive)) {
        // Only an immediate - means we should return negative infinity
        return first_code_point == '-' ? js_negative_infinity() : js_infinity();
    }

    return js_nan();
}

// 19.2.5 parseInt ( string, radix ), https://tc39.es/ecma262/#sec-parseint-string-radix
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::parse_int)
{
    // 1. Let inputString be ? ToString(string).
    auto input_string = TRY(vm.argument(0).to_deprecated_string(vm));

    // 2. Let S be ! TrimString(inputString, start).
    // NOTE: We TRY this operation only to propagate OOM errors.
    auto string = TRY(trim_string(vm, PrimitiveString::create(vm, input_string), TrimMode::Left));

    // 3. Let sign be 1.
    auto sign = 1;

    // 4. If S is not empty and the first code unit of S is the code unit 0x002D (HYPHEN-MINUS), set sign to -1.
    auto first_code_point = string.is_empty() ? OptionalNone {} : Optional<u32> { *string.code_points().begin() };
    if (first_code_point == 0x2Du)
        sign = -1;

    // 5. If S is not empty and the first code unit of S is the code unit 0x002B (PLUS SIGN) or the code unit 0x002D (HYPHEN-MINUS), remove the first code unit from S.
    auto trimmed_view = string.bytes_as_string_view();
    if (first_code_point == 0x2Bu || first_code_point == 0x2Du)
        trimmed_view = trimmed_view.substring_view(1);

    // 6. Let R be ℝ(? ToInt32(radix)).
    auto radix = TRY(vm.argument(1).to_i32(vm));

    // 7. Let stripPrefix be true.
    auto strip_prefix = true;

    // 8. If R ≠ 0, then
    if (radix != 0) {
        // a. If R < 2 or R > 36, return NaN.
        if (radix < 2 || radix > 36)
            return js_nan();

        // b. If R ≠ 16, set stripPrefix to false.
        if (radix != 16)
            strip_prefix = false;
    }
    // 9. Else,
    else {
        // a. Set R to 10.
        radix = 10;
    }

    // 10. If stripPrefix is true, then
    if (strip_prefix) {
        // a. If the length of S is at least 2 and the first two code units of S are either "0x" or "0X", then
        if (trimmed_view.length() >= 2 && trimmed_view.substring_view(0, 2).equals_ignoring_case("0x"sv)) {
            // i. Remove the first two code units from S.
            trimmed_view = trimmed_view.substring_view(2);

            // ii. Set R to 16.
            radix = 16;
        }
    }

    // 11. If S contains a code unit that is not a radix-R digit, let end be the index within S of the first such code unit; otherwise, let end be the length of S.
    // 12. Let Z be the substring of S from 0 to end.
    // 13. If Z is empty, return NaN.
    // 14. Let mathInt be the integer value that is represented by Z in radix-R notation, using the letters A-Z and a-z for digits with values 10 through 35. (However, if R is 10 and Z contains more than 20 significant digits, every significant digit after the 20th may be replaced by a 0 digit, at the option of the implementation; and if R is not 2, 4, 8, 10, 16, or 32, then mathInt may be an implementation-approximated integer representing the integer value denoted by Z in radix-R notation.)
    auto parse_digit = [&](u32 code_point) -> Optional<u32> {
        if (!is_ascii_alphanumeric(code_point))
            return {};
        auto digit = parse_ascii_base36_digit(code_point);
        if (digit >= (u32)radix)
            return {};
        return digit;
    };

    bool had_digits = false;
    double number = 0;
    for (auto code_point : Utf8View(trimmed_view)) {
        auto digit = parse_digit(code_point);
        if (!digit.has_value())
            break;
        had_digits = true;
        number *= radix;
        number += digit.value();
    }

    if (!had_digits)
        return js_nan();

    // 15. If mathInt = 0, then
    // a. If sign = -1, return -0𝔽.
    // b. Return +0𝔽.
    // 16. Return 𝔽(sign × mathInt).
    return Value(sign * number);
}

// 19.2.1 eval ( x ), https://tc39.es/ecma262/#sec-eval-x
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::eval)
{
    return perform_eval(vm, vm.argument(0), CallerMode::NonStrict, EvalMode::Indirect);
}

// 19.2.6.1.1 Encode ( string, unescapedSet ), https://tc39.es/ecma262/#sec-encode
static ThrowCompletionOr<DeprecatedString> encode(VM& vm, DeprecatedString const& string, StringView unescaped_set)
{
    auto utf16_string = TRY(Utf16String::create(vm, string));

    // 1. Let strLen be the length of string.
    auto string_length = utf16_string.length_in_code_units();

    // 2. Let R be the empty String.
    StringBuilder encoded_builder;

    // 3. Let k be 0.
    auto k = 0u;
    // 4. Repeat,
    while (k < string_length) {
        // a. If k = strLen, return R.
        // Handled below

        // b. Let C be the code unit at index k within string.
        auto code_unit = utf16_string.code_unit_at(k);
        // c. If C is in unescapedSet, then
        // NOTE: We assume the unescaped set only contains ascii characters as unescaped_set is a StringView.
        if (code_unit < 0x80 && unescaped_set.contains(static_cast<char>(code_unit))) {
            // i. Set k to k + 1.
            k++;

            // ii. Set R to the string-concatenation of R and C.
            encoded_builder.append(code_unit);
        }
        // d. Else,
        else {
            // i. Let cp be CodePointAt(string, k).
            auto code_point = code_point_at(utf16_string.view(), k);
            // ii. If cp.[[IsUnpairedSurrogate]] is true, throw a URIError exception.
            if (code_point.is_unpaired_surrogate)
                return vm.throw_completion<URIError>(ErrorType::URIMalformed);

            // iii. Set k to k + cp.[[CodeUnitCount]].
            k += code_point.code_unit_count;

            // iv. Let Octets be the List of octets resulting by applying the UTF-8 transformation to cp.[[CodePoint]].
            // v. For each element octet of Octets, do
            auto nwritten = AK::UnicodeUtils::code_point_to_utf8(code_point.code_point, [&encoded_builder](u8 octet) {
                // 1. Set R to the string-concatenation of:
                //  * R
                //  * "%"
                //  * the String representation of octet, formatted as a two-digit uppercase hexadecimal number, padded to the left with a zero if necessary
                encoded_builder.appendff("%{:02X}", octet);
            });
            VERIFY(nwritten > 0);
        }
    }
    return encoded_builder.build();
}

// 19.2.6.1.2 Decode ( string, reservedSet ), https://tc39.es/ecma262/#sec-decode
static ThrowCompletionOr<DeprecatedString> decode(VM& vm, DeprecatedString const& string, StringView reserved_set)
{
    StringBuilder decoded_builder;
    auto code_point_start_offset = 0u;
    auto expected_continuation_bytes = 0;
    for (size_t k = 0; k < string.length(); k++) {
        auto code_unit = string[k];
        if (code_unit != '%') {
            if (expected_continuation_bytes > 0)
                return vm.throw_completion<URIError>(ErrorType::URIMalformed);

            decoded_builder.append(code_unit);
            continue;
        }

        if (k + 2 >= string.length())
            return vm.throw_completion<URIError>(ErrorType::URIMalformed);

        auto first_digit = decode_hex_digit(string[k + 1]);
        if (first_digit >= 16)
            return vm.throw_completion<URIError>(ErrorType::URIMalformed);

        auto second_digit = decode_hex_digit(string[k + 2]);
        if (second_digit >= 16)
            return vm.throw_completion<URIError>(ErrorType::URIMalformed);

        u8 decoded_code_unit = (first_digit << 4) | second_digit;
        k += 2;
        if (expected_continuation_bytes > 0) {
            decoded_builder.append(decoded_code_unit);
            expected_continuation_bytes--;
            if (expected_continuation_bytes == 0 && !Utf8View(decoded_builder.string_view().substring_view(code_point_start_offset)).validate())
                return vm.throw_completion<URIError>(ErrorType::URIMalformed);
            continue;
        }

        if (decoded_code_unit < 0x80) {
            if (reserved_set.contains(static_cast<char>(decoded_code_unit)))
                decoded_builder.append(string.substring_view(k - 2, 3));
            else
                decoded_builder.append(decoded_code_unit);
            continue;
        }

        auto leading_ones = count_leading_zeroes_safe(static_cast<u8>(~decoded_code_unit));
        if (leading_ones == 1 || leading_ones > 4)
            return vm.throw_completion<URIError>(ErrorType::URIMalformed);

        code_point_start_offset = decoded_builder.length();
        decoded_builder.append(decoded_code_unit);
        expected_continuation_bytes = leading_ones - 1;
    }
    if (expected_continuation_bytes > 0)
        return vm.throw_completion<URIError>(ErrorType::URIMalformed);
    return decoded_builder.build();
}

// 19.2.6.4 encodeURI ( uri ), https://tc39.es/ecma262/#sec-encodeuri-uri
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::encode_uri)
{
    auto uri_string = TRY(vm.argument(0).to_deprecated_string(vm));
    auto encoded = TRY(encode(vm, uri_string, ";/?:@&=+$,abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.!~*'()#"sv));
    return PrimitiveString::create(vm, move(encoded));
}

// 19.2.6.2 decodeURI ( encodedURI ), https://tc39.es/ecma262/#sec-decodeuri-encodeduri
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::decode_uri)
{
    auto uri_string = TRY(vm.argument(0).to_deprecated_string(vm));
    auto decoded = TRY(decode(vm, uri_string, ";/?:@&=+$,#"sv));
    return PrimitiveString::create(vm, move(decoded));
}

// 19.2.6.5 encodeURIComponent ( uriComponent ), https://tc39.es/ecma262/#sec-encodeuricomponent-uricomponent
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::encode_uri_component)
{
    auto uri_string = TRY(vm.argument(0).to_deprecated_string(vm));
    auto encoded = TRY(encode(vm, uri_string, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.!~*'()"sv));
    return PrimitiveString::create(vm, move(encoded));
}

// 19.2.6.3 decodeURIComponent ( encodedURIComponent ), https://tc39.es/ecma262/#sec-decodeuricomponent-encodeduricomponent
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::decode_uri_component)
{
    auto uri_string = TRY(vm.argument(0).to_deprecated_string(vm));
    auto decoded = TRY(decode(vm, uri_string, ""sv));
    return PrimitiveString::create(vm, move(decoded));
}

// B.2.1.1 escape ( string ), https://tc39.es/ecma262/#sec-escape-string
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::escape)
{
    auto string = TRY(vm.argument(0).to_deprecated_string(vm));
    StringBuilder escaped;
    for (auto code_point : TRY_OR_THROW_OOM(vm, utf8_to_utf16(string))) {
        if (code_point < 256) {
            if ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@*_+-./"sv.contains(static_cast<char>(code_point)))
                escaped.append(code_point);
            else
                escaped.appendff("%{:02X}", code_point);
            continue;
        }
        escaped.appendff("%u{:04X}", code_point);
    }
    return PrimitiveString::create(vm, escaped.build());
}

// B.2.1.2 unescape ( string ), https://tc39.es/ecma262/#sec-unescape-string
JS_DEFINE_NATIVE_FUNCTION(GlobalObject::unescape)
{
    auto string = TRY(vm.argument(0).to_deprecated_string(vm));
    ssize_t length = string.length();
    StringBuilder unescaped(length);
    for (auto k = 0; k < length; ++k) {
        u32 code_point = string[k];
        if (code_point == '%') {
            if (k <= length - 6 && string[k + 1] == 'u' && is_ascii_hex_digit(string[k + 2]) && is_ascii_hex_digit(string[k + 3]) && is_ascii_hex_digit(string[k + 4]) && is_ascii_hex_digit(string[k + 5])) {
                code_point = (parse_ascii_hex_digit(string[k + 2]) << 12) | (parse_ascii_hex_digit(string[k + 3]) << 8) | (parse_ascii_hex_digit(string[k + 4]) << 4) | parse_ascii_hex_digit(string[k + 5]);
                k += 5;
            } else if (k <= length - 3 && is_ascii_hex_digit(string[k + 1]) && is_ascii_hex_digit(string[k + 2])) {
                code_point = (parse_ascii_hex_digit(string[k + 1]) << 4) | parse_ascii_hex_digit(string[k + 2]);
                k += 2;
            }
        }
        unescaped.append_code_point(code_point);
    }
    return PrimitiveString::create(vm, unescaped.build());
}

}
