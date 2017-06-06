#include <erl_nif.h>
#include <phonenumbers/phonenumberutil.h>
#include <phonenumbers/phonenumber.pb.h>
#include <memory>
#include <iostream>
#include <set>
#include <string>

using namespace i18n::phonenumbers;

struct atoms
{
	ERL_NIF_TERM atomTrue;
	ERL_NIF_TERM atomFalse;

	ERL_NIF_TERM atomFixedLine;
	ERL_NIF_TERM atomMobile;
	ERL_NIF_TERM atomFixedLineOrMobile;
	ERL_NIF_TERM atomTollFree;
	ERL_NIF_TERM atomPremiumRate;
	ERL_NIF_TERM atomSharedCost;
	ERL_NIF_TERM atomVoip;
	ERL_NIF_TERM atomPersonalNumber;
	ERL_NIF_TERM atomPager;
	ERL_NIF_TERM atomUan;
	ERL_NIF_TERM atomVoiceMail;
	ERL_NIF_TERM atomUnknown;

	ERL_NIF_TERM atomIsPossible;
  ERL_NIF_TERM atomIsPossibleLocalOnly;
	ERL_NIF_TERM atomInvalidContryCode;
  ERL_NIF_TERM atomInvalidLength;
	ERL_NIF_TERM atomTooShort;
	ERL_NIF_TERM atomTooLong;

	ERL_NIF_TERM atomFromNumberWithPlusSign;
	ERL_NIF_TERM atomFromNumberWithIdd;
	ERL_NIF_TERM atomFromNumberWithoutPlusSign;
	ERL_NIF_TERM atomFromDefaultCountry;

	ERL_NIF_TERM atomInvalidNumber;
	ERL_NIF_TERM atomNoMatch;
	ERL_NIF_TERM atomShortNsmMatch;
	ERL_NIF_TERM atomNsmMatch;
	ERL_NIF_TERM atomExactMatch;

	ERL_NIF_TERM atomFormatE164;
	ERL_NIF_TERM atomFormatInternational;
	ERL_NIF_TERM atomFormatNational;
	ERL_NIF_TERM atomFormatRFC3966;

	ERL_NIF_TERM atomPhoneNumber;

} ATOMS;

ERL_NIF_TERM make_atom(ErlNifEnv* env, const char* name)
{
    ERL_NIF_TERM ret;

    if(enif_make_existing_atom(env, name, &ret, ERL_NIF_LATIN1))
        return ret;

    return enif_make_atom(env, name);
}

int on_nif_load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info)
{
	ATOMS.atomTrue = make_atom(env, "true");
	ATOMS.atomFalse = make_atom(env, "false");

	ATOMS.atomFixedLine = make_atom(env, "fixed_line");
	ATOMS.atomMobile = make_atom(env, "mobile");
	ATOMS.atomFixedLineOrMobile = make_atom(env, "fixed_line_or_mobile");
	ATOMS.atomTollFree = make_atom(env, "toll_free");
	ATOMS.atomPremiumRate = make_atom(env, "premium_rate");
	ATOMS.atomSharedCost = make_atom(env, "shared_cost");
	ATOMS.atomVoip = make_atom(env, "voip");
	ATOMS.atomPersonalNumber = make_atom(env, "personal_number");
	ATOMS.atomPager = make_atom(env, "pager");
	ATOMS.atomUan = make_atom(env, "uan");
	ATOMS.atomVoiceMail = make_atom(env, "voicemail");
	ATOMS.atomUnknown = make_atom(env, "unknown");

	ATOMS.atomIsPossible = make_atom(env, "is_possible");
  ATOMS.atomIsPossibleLocalOnly = make_atom(env, "is_possible_local_only");
	ATOMS.atomInvalidContryCode = make_atom(env, "invalid_country_code");
  ATOMS.atomInvalidLength = make_atom(env, "invalid_length");
	ATOMS.atomTooShort = make_atom(env, "too_short");
	ATOMS.atomTooLong = make_atom(env, "too_long");

	ATOMS.atomFromNumberWithPlusSign = make_atom(env, "from_number_with_plus_sign");
	ATOMS.atomFromNumberWithIdd = make_atom(env, "from_number_with_idd");
	ATOMS.atomFromNumberWithoutPlusSign = make_atom(env, "from_number_without_plus_sign");
	ATOMS.atomFromDefaultCountry = make_atom(env, "from_default_country");

	ATOMS.atomInvalidNumber = make_atom(env, "invalid_number");
	ATOMS.atomNoMatch = make_atom(env, "no_match");
	ATOMS.atomShortNsmMatch = make_atom(env, "short_nsn_match");
	ATOMS.atomNsmMatch = make_atom(env, "nsn_match");
	ATOMS.atomExactMatch = make_atom(env, "exact_match");

	ATOMS.atomFormatE164 = make_atom(env, "e164");
	ATOMS.atomFormatInternational = make_atom(env, "international");
	ATOMS.atomFormatNational = make_atom(env, "national");
	ATOMS.atomFormatRFC3966 = make_atom(env, "rfc3966");

	ATOMS.atomPhoneNumber = make_atom(env, "phonenumber");

	*priv_data = NULL;
	return 0;
}

static bool term_to_phonenumber_format(const ERL_NIF_TERM term, PhoneNumberUtil::PhoneNumberFormat* number_format)
{
	bool return_value = true;

	if(enif_is_identical(ATOMS.atomFormatE164, term))
		*number_format = PhoneNumberUtil::E164;
	else if(enif_is_identical(ATOMS.atomFormatInternational, term))
		*number_format = PhoneNumberUtil::INTERNATIONAL;
	else if(enif_is_identical(ATOMS.atomFormatNational, term))
		*number_format = PhoneNumberUtil::NATIONAL;
	else if(enif_is_identical(ATOMS.atomFormatRFC3966, term))
		*number_format = PhoneNumberUtil::RFC3966;
	else
		return_value = false;

    return return_value;
}

static bool term_to_phonenumber_country_code_source(const ERL_NIF_TERM term, PhoneNumber::CountryCodeSource* country_code_source)
{
	bool return_value = true;

	if(enif_is_identical(ATOMS.atomFromNumberWithPlusSign, term))
		*country_code_source = PhoneNumber::FROM_NUMBER_WITH_PLUS_SIGN;
	else if(enif_is_identical(ATOMS.atomFromNumberWithIdd, term))
		*country_code_source = PhoneNumber::FROM_NUMBER_WITH_IDD;
	else if(enif_is_identical(ATOMS.atomFromNumberWithoutPlusSign, term))
		*country_code_source = PhoneNumber::FROM_NUMBER_WITHOUT_PLUS_SIGN;
	else if(enif_is_identical(ATOMS.atomFromDefaultCountry, term))
		*country_code_source = PhoneNumber::FROM_DEFAULT_COUNTRY;
	else
		return_value = false;

	return return_value;
}

static bool term_to_phonenumber_type(const ERL_NIF_TERM term, PhoneNumberUtil::PhoneNumberType* type)
{
	bool return_value = true;

	if(enif_is_identical(ATOMS.atomFixedLine, term))
		*type = PhoneNumberUtil::FIXED_LINE;
	else if(enif_is_identical(ATOMS.atomMobile, term))
		*type = PhoneNumberUtil::MOBILE;
	else if(enif_is_identical(ATOMS.atomFixedLineOrMobile, term))
		*type = PhoneNumberUtil::FIXED_LINE_OR_MOBILE;
	else if(enif_is_identical(ATOMS.atomTollFree, term))
		*type = PhoneNumberUtil::TOLL_FREE;
	else if(enif_is_identical(ATOMS.atomPremiumRate, term))
		*type = PhoneNumberUtil::PREMIUM_RATE;
	else if(enif_is_identical(ATOMS.atomSharedCost, term))
		*type = PhoneNumberUtil::SHARED_COST;
	else if(enif_is_identical(ATOMS.atomVoip, term))
		*type = PhoneNumberUtil::VOIP;
	else if(enif_is_identical(ATOMS.atomPersonalNumber, term))
		*type = PhoneNumberUtil::PERSONAL_NUMBER;
	else if(enif_is_identical(ATOMS.atomPager, term))
		*type = PhoneNumberUtil::PAGER;
	else if(enif_is_identical(ATOMS.atomUan, term))
			*type = PhoneNumberUtil::UAN;
	else if(enif_is_identical(ATOMS.atomVoiceMail, term))
		*type = PhoneNumberUtil::VOICEMAIL;
	else if(enif_is_identical(ATOMS.atomUnknown, term))
		*type = PhoneNumberUtil::UNKNOWN;
	else
		return_value = false;

	return return_value;
}

static bool term_to_boolean(const ERL_NIF_TERM term, bool* boolean)
{
	if(enif_is_identical(term, ATOMS.atomTrue))
	{
		*boolean = true;
		return true;
	}

	if(enif_is_identical(term, ATOMS.atomFalse))
	{
		*boolean = false;
		return true;
	}

	return false;
}

static ERL_NIF_TERM boolean_to_term(bool boolean)
{
	return boolean ? ATOMS.atomTrue : ATOMS.atomFalse;
}

static ERL_NIF_TERM phonenumber_type_to_term(PhoneNumberUtil::PhoneNumberType type)
{
	 switch(type)
	 {
		case PhoneNumberUtil::FIXED_LINE:
			return ATOMS.atomFixedLine;
		case PhoneNumberUtil::MOBILE:
			return ATOMS.atomMobile;
		case PhoneNumberUtil::FIXED_LINE_OR_MOBILE:
			return ATOMS.atomFixedLineOrMobile;
		case PhoneNumberUtil::TOLL_FREE:
			return ATOMS.atomTollFree;
		case PhoneNumberUtil::PREMIUM_RATE:
			return ATOMS.atomPremiumRate;
		case PhoneNumberUtil::SHARED_COST:
			return ATOMS.atomSharedCost;
		case PhoneNumberUtil::VOIP:
			return ATOMS.atomVoip;
		case PhoneNumberUtil::PERSONAL_NUMBER:
			return ATOMS.atomPersonalNumber;
		case PhoneNumberUtil::PAGER:
			return ATOMS.atomPager;
		case PhoneNumberUtil::UAN:
			return ATOMS.atomUan;
		case PhoneNumberUtil::VOICEMAIL:
			return ATOMS.atomVoiceMail;
		case PhoneNumberUtil::UNKNOWN:
			return ATOMS.atomUnknown;
	 }

	return ATOMS.atomUnknown;
}

static ERL_NIF_TERM phonenumber_validation_result_to_term(PhoneNumberUtil::ValidationResult validation_result)
{
    switch(validation_result)
    {
        case PhoneNumberUtil::IS_POSSIBLE:
            return ATOMS.atomIsPossible;
        case PhoneNumberUtil::INVALID_COUNTRY_CODE:
        	return ATOMS.atomInvalidContryCode;
        case PhoneNumberUtil::TOO_SHORT:
        	return ATOMS.atomTooShort;
        case PhoneNumberUtil::TOO_LONG:
        	return ATOMS.atomTooLong;
       #ifdef LPN_HASLOCALONLY
        case PhoneNumberUtil::IS_POSSIBLE_LOCAL_ONLY:
          return ATOMS.atomIsPossibleLocalOnly;
        case PhoneNumberUtil::INVALID_LENGTH:
          return ATOMS.atomInvalidLength;
       #endif
    }

    return ATOMS.atomInvalidContryCode;
}

static ERL_NIF_TERM phonenumber_country_code_source_to_term(ErlNifEnv* env, PhoneNumber::CountryCodeSource country_code_source)
{
    switch(country_code_source)
    {
        case PhoneNumber::FROM_NUMBER_WITH_PLUS_SIGN:
            return ATOMS.atomFromNumberWithPlusSign;
        case PhoneNumber::FROM_NUMBER_WITH_IDD:
        	return ATOMS.atomFromNumberWithIdd;
        case PhoneNumber::FROM_NUMBER_WITHOUT_PLUS_SIGN:
        	return ATOMS.atomFromNumberWithoutPlusSign;
        case PhoneNumber::FROM_DEFAULT_COUNTRY:
        	return ATOMS.atomFromDefaultCountry;
    }

    return ATOMS.atomFromDefaultCountry;
}

static ERL_NIF_TERM phonenumber_match_type_to_term(PhoneNumberUtil::MatchType match_type)
{
    switch(match_type)
    {
        case PhoneNumberUtil::INVALID_NUMBER:
            return ATOMS.atomInvalidNumber;
        case PhoneNumberUtil::NO_MATCH:
        	return ATOMS.atomNoMatch;
        case PhoneNumberUtil::SHORT_NSN_MATCH:
        	return ATOMS.atomShortNsmMatch;
        case PhoneNumberUtil::NSN_MATCH:
        	return ATOMS.atomNsmMatch;
        case PhoneNumberUtil::EXACT_MATCH:
        	return ATOMS.atomExactMatch;
    }

    return ATOMS.atomInvalidNumber;
}

static ERL_NIF_TERM phonenumber_to_term(ErlNifEnv* env, PhoneNumber phoneNumber)
{
    unsigned char *buffer;
    ERL_NIF_TERM raw_input;
    ERL_NIF_TERM extension;
    ERL_NIF_TERM preferred_domestic_carrier_code;

    // required int32 country_code = 1;
    ERL_NIF_TERM has_country_code = boolean_to_term(phoneNumber.has_country_code());
    ERL_NIF_TERM country_code = enif_make_int(env, phoneNumber.country_code());

    // required uint64 national_number = 2;
    ERL_NIF_TERM has_national_number = boolean_to_term(phoneNumber.has_national_number());
    ERL_NIF_TERM national_number = enif_make_ulong(env, phoneNumber.national_number());

    // optional string extension = 3;
    ERL_NIF_TERM has_extension = boolean_to_term(phoneNumber.has_extension());
    buffer = enif_make_new_binary(env, phoneNumber.extension().size(), &extension);
    std::copy(phoneNumber.extension().begin(), phoneNumber.extension().end(), buffer);

    // optional bool italian_leading_zero = 4;
    ERL_NIF_TERM has_italian_leading_zero = boolean_to_term(phoneNumber.has_italian_leading_zero());
    ERL_NIF_TERM italian_leading_zero = boolean_to_term(phoneNumber.italian_leading_zero());

    // optional int32 number_of_leading_zeros = 8 [default = 1];
    ERL_NIF_TERM has_number_of_leading_zeros = boolean_to_term(phoneNumber.has_number_of_leading_zeros());
    ERL_NIF_TERM number_of_leading_zeros = enif_make_int(env, phoneNumber.number_of_leading_zeros());
    
    // optional string raw_input = 5;
    ERL_NIF_TERM has_raw_input = boolean_to_term(phoneNumber.has_raw_input());
    buffer = enif_make_new_binary(env, phoneNumber.raw_input().size(), &raw_input);
    std::copy(phoneNumber.raw_input().begin(), phoneNumber.raw_input().end(), buffer);

    // optional .i18n.phonenumbers.PhoneNumber.CountryCodeSource country_code_source = 6;
    ERL_NIF_TERM has_country_code_source = boolean_to_term(phoneNumber.has_country_code_source());
    ERL_NIF_TERM country_code_source = phonenumber_country_code_source_to_term(env, phoneNumber.country_code_source());

    // optional string preferred_domestic_carrier_code = 7;
    ERL_NIF_TERM has_preferred_domestic_carrier_code = boolean_to_term(phoneNumber.has_preferred_domestic_carrier_code());
    buffer = enif_make_new_binary(env, phoneNumber.preferred_domestic_carrier_code().size(), &preferred_domestic_carrier_code);
    std::copy(phoneNumber.preferred_domestic_carrier_code().begin(), phoneNumber.preferred_domestic_carrier_code().end(), buffer);

    return enif_make_tuple(env, 17,
        ATOMS.atomPhoneNumber,
        has_country_code,
        country_code,
        has_national_number,
        national_number,
        has_extension,
        extension,
        has_number_of_leading_zeros,
        number_of_leading_zeros,
        has_italian_leading_zero,
        italian_leading_zero,
        has_raw_input,
        raw_input,
        has_country_code_source,
        country_code_source,
        has_preferred_domestic_carrier_code,
        preferred_domestic_carrier_code);
}

static bool term_to_phonenumber(ErlNifEnv* env, const ERL_NIF_TERM term, PhoneNumber* phoneNumber)
{
    int integer;
    unsigned long longer; //FIXME should be unsingned long long
    bool boolean;
    ErlNifBinary bin;
    int PhoneNumberElems;

    const ERL_NIF_TERM* array;

    // Get Term
    if (!enif_get_tuple(env, term, &PhoneNumberElems, &array) || PhoneNumberElems != 17)
        return false;

    // required int32 country_code = 1;
    if (!enif_get_int(env, array[2], &integer))
        return false;

    phoneNumber->set_country_code(integer);

    // required uint64 national_number = 2;
    if (!enif_get_ulong(env, array[4], &longer))
        return false;

    phoneNumber->set_national_number(longer);

    // optional string extension = 3;

    if (!term_to_boolean(array[5], &boolean))
        return false;

    if (boolean)
    {
        if (!enif_inspect_iolist_as_binary(env, array[6], &bin))
            return false;

        phoneNumber->set_extension(std::string ( (char*) bin.data, bin.size));
    }

    // optional int32 number_of_leading_zeros = 8 [default = 1];
    if (!term_to_boolean(array[7], &boolean))
        return false;

    if (boolean)
    {
        if (!enif_get_int(env, array[8], &integer))
            return false;

        phoneNumber->set_number_of_leading_zeros(integer);
    }

    // optional bool italian_leading_zero = 4;
    if (!term_to_boolean(array[9], &boolean))
        return false;

    if (boolean)
    {
        if (!term_to_boolean(array[10], &boolean))
            return false;

        phoneNumber->set_italian_leading_zero(boolean);
    }

    // optional string raw_input = 5;
    if (!term_to_boolean(array[11], &boolean))
        return false;

    if (boolean)
    {
        if (!enif_inspect_iolist_as_binary(env, array[12], &bin))
            return false;

        phoneNumber->set_raw_input(std::string ( (char*) bin.data, bin.size));
    }

    // optional .i18n.phonenumbers.PhoneNumber.CountryCodeSource country_code_source = 6;
    if (!term_to_boolean(array[13], &boolean))
        return false;

    if (boolean)
    {
        PhoneNumber::CountryCodeSource country_code_source;
        if (!term_to_phonenumber_country_code_source(array[14], &country_code_source))
            return false;

        phoneNumber->set_country_code_source(country_code_source);
    }

    // Get Preferred Domestic Carrier Code
    if (!term_to_boolean(array[15], &boolean))
        return false;

    if (boolean)
    {
        if (!enif_inspect_iolist_as_binary(env, array[16], &bin))
            return false;

        phoneNumber->set_preferred_domestic_carrier_code(std::string ( (char*) bin.data, bin.size));
    }

    return true;
}

// NIF functions

static ERL_NIF_TERM GetSupportedRegions_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    set<string> regions;
    phone_util_->GetSupportedRegions(&regions);
    unsigned int cnt = regions.size();
    ERL_NIF_TERM arr[cnt];

    unsigned int i = 0;
    ERL_NIF_TERM ret;
    unsigned char *region;

    for (set<string>::iterator it=regions.begin(); it!=regions.end(); ++it, i++)
    {
        region = enif_make_new_binary(env, it->size(), &ret);
        std::copy(it->begin(), it->end(), region);
        arr[i] = ret;
        ret = 0;
    }

    return enif_make_list_from_array(env, arr, cnt);
}

static ERL_NIF_TERM IsAlphaNumber_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    std::string str( (char*) bin.data, bin.size);
    return boolean_to_term(phone_util_->IsAlphaNumber(str));
}

static ERL_NIF_TERM ConvertAlphaCharactersInNumber_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    std::string str( (char*) bin.data, bin.size);
    phone_util_->ConvertAlphaCharactersInNumber(&str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM NormalizeDigitsOnly_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    std::string str( (char*) bin.data, bin.size);
    phone_util_->NormalizeDigitsOnly(&str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM NormalizeDiallableCharsOnly_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    std::string str( (char*) bin.data, bin.size);
    phone_util_->NormalizeDiallableCharsOnly(&str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM GetNationalSignificantNumber_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string str;
    phone_util_->GetNationalSignificantNumber(phoneNumber, &str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM GetLengthOfGeographicalAreaCode_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    return enif_make_int(env, phone_util_->GetLengthOfGeographicalAreaCode(phoneNumber));
}

static ERL_NIF_TERM GetLengthOfNationalDestinationCode_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    return enif_make_int(env, phone_util_->GetLengthOfNationalDestinationCode(phoneNumber));
}

static ERL_NIF_TERM GetCountryMobileToken_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int code;
    if (!enif_get_int(env, argv[0], &code))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    std::string mobile_token;
    
    phone_util_->GetCountryMobileToken(code, &mobile_token);

    ERL_NIF_TERM ret;
    unsigned char *token = enif_make_new_binary(env, mobile_token.size(), &ret);
    std::copy(mobile_token.begin(), mobile_token.end(), token);
    
    return ret;
}

static ERL_NIF_TERM Format_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil::PhoneNumberFormat phoneNumberFormat;
    if (!term_to_phonenumber_format(argv[1], &phoneNumberFormat))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string str;
    phone_util_->Format(phoneNumber, phoneNumberFormat, &str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM FormatNationalNumberWithPreferredCarrierCode_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string fb_carrier_code( (char*) bin.data, bin.size);
    std::string str;
    phone_util_->FormatNationalNumberWithPreferredCarrierCode(phoneNumber, fb_carrier_code, &str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM FormatNationalNumberWithCarrierCode_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string carrier_code( (char*) bin.data, bin.size);
    std::string str;
    phone_util_->FormatNationalNumberWithCarrierCode(phoneNumber, carrier_code, &str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM FormatNumberForMobileDialing_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &bin))
        return enif_make_badarg(env);

    bool with_formatting;
    if (!term_to_boolean(argv[2], &with_formatting))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string region_calling_from( (char*) bin.data, bin.size);
    std::string str;
    phone_util_->FormatNumberForMobileDialing(phoneNumber, region_calling_from, with_formatting, &str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM FormatOutOfCountryCallingNumber_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string calling_from( (char*) bin.data, bin.size);
    std::string str;
    phone_util_->FormatOutOfCountryCallingNumber(phoneNumber, calling_from, &str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM FormatInOriginalFormat_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string region_code( (char*) bin.data, bin.size);
    std::string str;
    phone_util_->FormatInOriginalFormat(phoneNumber, region_code, &str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM FormatOutOfCountryKeepingAlphaChars_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string calling_from( (char*) bin.data, bin.size);
    std::string str;
    phone_util_->FormatOutOfCountryKeepingAlphaChars(phoneNumber, calling_from, &str);

    ERL_NIF_TERM ret;
    unsigned char *number = enif_make_new_binary(env, str.size(), &ret);
    std::copy(str.begin(), str.end(), number);
    
    return ret;
}

static ERL_NIF_TERM TruncateTooLongNumber_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    phone_util_->TruncateTooLongNumber(&phoneNumber);

    return phonenumber_to_term(env, phoneNumber);
}

static ERL_NIF_TERM GetNumberType_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    return phonenumber_type_to_term(phone_util_->GetNumberType(phoneNumber));
}

static ERL_NIF_TERM IsValidNumber_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    return boolean_to_term(phone_util_->IsValidNumber(phoneNumber));
}

static ERL_NIF_TERM IsValidNumberForRegion_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &bin))
        return enif_make_badarg(env);

    std::string region_code( (char*) bin.data, bin.size);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    return boolean_to_term(phone_util_->IsValidNumberForRegion(phoneNumber, region_code));
}

static ERL_NIF_TERM GetRegionCodeForNumber_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    std::string region_code;
    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    phone_util_->GetRegionCodeForNumber(phoneNumber, &region_code);

    ERL_NIF_TERM ret;
    unsigned char *region = enif_make_new_binary(env, region_code.size(), &ret);
    std::copy(region_code.begin(), region_code.end(), region);
    
    return ret;
}

static ERL_NIF_TERM GetCountryCodeForRegion_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string region_code( (char*) bin.data, bin.size);
    
    return enif_make_int(env, phone_util_->GetCountryCodeForRegion(region_code));
}

static ERL_NIF_TERM GetRegionCodeForCountryCode_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int code;
    if (!enif_get_int(env, argv[0], &code))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    std::string region_code;
    
    phone_util_->GetRegionCodeForCountryCode(code, &region_code);

    ERL_NIF_TERM ret;
    unsigned char *region = enif_make_new_binary(env, region_code.size(), &ret);
    std::copy(region_code.begin(), region_code.end(), region);
    
    return ret;
}

static ERL_NIF_TERM GetRegionCodesForCountryCallingCode_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int code;
    if (!enif_get_int(env, argv[0], &code))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    list<string> regions;

    phone_util_->GetRegionCodesForCountryCallingCode(code, &regions);
    unsigned int cnt = regions.size();
    ERL_NIF_TERM arr[cnt];

    unsigned int i = 0;
    ERL_NIF_TERM ret;
    unsigned char *region;

    for (list<string>::iterator it=regions.begin(); it!=regions.end(); ++it, i++)
    {
        region = enif_make_new_binary(env, it->size(), &ret);
        std::copy(it->begin(), it->end(), region);
        arr[i] = ret;
        ret = 0;
    }

    return enif_make_list_from_array(env, arr, cnt);
}

static ERL_NIF_TERM IsNANPACountry_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary regionCodeNif;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &regionCodeNif))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string region_code( (char*) regionCodeNif.data, regionCodeNif.size);
    return boolean_to_term(phone_util_->IsNANPACountry(region_code));
}

static ERL_NIF_TERM GetNddPrefixForRegion_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary regionCodeNif;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &regionCodeNif))
        return enif_make_badarg(env);

    bool stripNonDigits;
    if (!term_to_boolean(argv[1], &stripNonDigits))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    std::string region_code( (char*) regionCodeNif.data, regionCodeNif.size);
    std::string national_prefix;
    phone_util_->GetNddPrefixForRegion(region_code, stripNonDigits, &national_prefix);

    ERL_NIF_TERM ret;
    unsigned char *prefix = enif_make_new_binary(env, national_prefix.size(), &ret);
    std::copy(national_prefix.begin(), national_prefix.end(), prefix);
    
    return ret;
}

static ERL_NIF_TERM IsPossibleNumberWithReason_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    PhoneNumberUtil::ValidationResult validation_result = phone_util_->IsPossibleNumberWithReason(phoneNumber);

    return phonenumber_validation_result_to_term(validation_result);
}

static ERL_NIF_TERM IsPossibleNumber_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    return boolean_to_term(phone_util_->IsPossibleNumber(phoneNumber));
}

static ERL_NIF_TERM IsPossibleNumberForString_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary nifNumber;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &nifNumber))
        return enif_make_badarg(env);

    ErlNifBinary nifRegionDialingFrom;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &nifRegionDialingFrom))
        return enif_make_badarg(env);

    PhoneNumber phoneNumber;

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string region_dialing_from( (char*) nifRegionDialingFrom.data, nifRegionDialingFrom.size);
    std::string number( (char*) nifNumber.data, nifNumber.size);

    return boolean_to_term(phone_util_->IsPossibleNumberForString(number, region_dialing_from));
}

static ERL_NIF_TERM GetExampleNumber_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &bin))
        return enif_make_badarg(env);

    PhoneNumber phoneNumber;
    std::string region_code( (char*) bin.data, bin.size);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    if (phone_util_->GetExampleNumber(region_code, &phoneNumber))
        return phonenumber_to_term(env, phoneNumber);

    return ATOMS.atomFalse;
}

static ERL_NIF_TERM GetExampleNumberForType_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary bin;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &bin))
        return enif_make_badarg(env);

    PhoneNumberUtil::PhoneNumberType type;
    if (!term_to_phonenumber_type(argv[1], &type))
        return enif_make_badarg(env);

    PhoneNumber phoneNumber;
    std::string region_code( (char*) bin.data, bin.size);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    if (phone_util_->GetExampleNumberForType(region_code, type, &phoneNumber))
        return phonenumber_to_term(env, phoneNumber);

    return ATOMS.atomFalse;
}

static ERL_NIF_TERM GetExampleNumberForNonGeoEntity_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    int code;
    if (!enif_get_int(env, argv[0], &code))
        return enif_make_badarg(env);

    PhoneNumber phoneNumber;

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();

    if (phone_util_->GetExampleNumberForNonGeoEntity(code, &phoneNumber))
        return phonenumber_to_term(env, phoneNumber);

    return ATOMS.atomFalse;
}

static ERL_NIF_TERM Parse_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary numberToParse;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &numberToParse))
        return enif_make_badarg(env);

    ErlNifBinary defaultRegion;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &defaultRegion))
        return enif_make_badarg(env);

    PhoneNumber phoneNumber;

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string region_code( (char*) defaultRegion.data, defaultRegion.size);
    std::string number_to_parse( (char*) numberToParse.data, numberToParse.size);
    phone_util_->Parse(number_to_parse, region_code, &phoneNumber);

    return phonenumber_to_term(env, phoneNumber);
}

static ERL_NIF_TERM ParseAndKeepRawInput_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary numberToParse;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &numberToParse))
        return enif_make_badarg(env);

    ErlNifBinary defaultRegion;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &defaultRegion))
        return enif_make_badarg(env);

    PhoneNumber phoneNumber;

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string region_code( (char*) defaultRegion.data, defaultRegion.size);
    std::string number_to_parse( (char*) numberToParse.data, numberToParse.size);
    phone_util_->ParseAndKeepRawInput(number_to_parse, region_code, &phoneNumber);

    return phonenumber_to_term(env, phoneNumber);
}

static ERL_NIF_TERM IsNumberMatch_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber1;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber1))
        return enif_make_badarg(env);

    PhoneNumber phoneNumber2;
    if (!term_to_phonenumber(env, argv[1], &phoneNumber2))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    PhoneNumberUtil::MatchType match_type = phone_util_->IsNumberMatch(phoneNumber1, phoneNumber2);

    return phonenumber_match_type_to_term(match_type);
}

static ERL_NIF_TERM IsNumberMatchWithTwoStrings_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary nifFirstNumber;
    if (!enif_inspect_iolist_as_binary(env, argv[0], &nifFirstNumber))
        return enif_make_badarg(env);

    ErlNifBinary nifSecondNumber;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &nifSecondNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string first_number( (char*) nifFirstNumber.data, nifFirstNumber.size);
    std::string second_number( (char*) nifSecondNumber.data, nifSecondNumber.size);
    PhoneNumberUtil::MatchType match_type =phone_util_->IsNumberMatchWithTwoStrings(first_number, second_number);

    return phonenumber_match_type_to_term(match_type);
}

static ERL_NIF_TERM IsNumberMatchWithOneString_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    PhoneNumber phoneNumber1;
    if (!term_to_phonenumber(env, argv[0], &phoneNumber1))
        return enif_make_badarg(env);

    ErlNifBinary nifSecondNumber;
    if (!enif_inspect_iolist_as_binary(env, argv[1], &nifSecondNumber))
        return enif_make_badarg(env);

    PhoneNumberUtil *phone_util_ = PhoneNumberUtil::GetInstance();
    std::string second_number( (char*) nifSecondNumber.data, nifSecondNumber.size);
    PhoneNumberUtil::MatchType match_type = phone_util_->IsNumberMatchWithOneString(phoneNumber1, second_number);

    return phonenumber_match_type_to_term(match_type);
}

static ErlNifFunc nif_funcs[] =
{
    {"get_supported_regions", 0, GetSupportedRegions_nif},
    {"is_alpha_number", 1, IsAlphaNumber_nif},
    {"convert_alpha_characters_in_number", 1, ConvertAlphaCharactersInNumber_nif},
    {"normalize_digits_only", 1, NormalizeDigitsOnly_nif},
    {"normalize_diallable_chars_only", 1, NormalizeDiallableCharsOnly_nif},
    {"get_national_significant_number", 1, GetNationalSignificantNumber_nif},
    {"get_length_of_geograpical_area_code", 1, GetLengthOfGeographicalAreaCode_nif},
    {"get_length_of_national_destination_code", 1, GetLengthOfNationalDestinationCode_nif},
    {"get_country_mobile_token", 1, GetCountryMobileToken_nif},
    {"format", 2, Format_nif},
    {"format_national_number_with_carrier_code", 2, FormatNationalNumberWithCarrierCode_nif},
    {"format_national_number_with_preferred_carrier_code", 2, FormatNationalNumberWithPreferredCarrierCode_nif},
    {"format_number_for_mobile_dialing", 3, FormatNumberForMobileDialing_nif},
    {"format_out_of_country_calling_number", 2, FormatOutOfCountryCallingNumber_nif},
    {"format_in_original_format", 2, FormatInOriginalFormat_nif},
    {"format_out_of_country_keeping_alpha_chars", 2, FormatOutOfCountryKeepingAlphaChars_nif},
    {"truncate_too_long_number", 1, TruncateTooLongNumber_nif},
    {"get_number_type", 1, GetNumberType_nif},
    {"is_valid_number", 1, IsValidNumber_nif},
    {"is_valid_number_for_region", 2, IsValidNumberForRegion_nif},
    {"get_region_code_for_number", 1, GetRegionCodeForNumber_nif},
    {"get_country_code_for_region", 1, GetCountryCodeForRegion_nif},
    {"get_region_code_for_country_code", 1, GetRegionCodeForCountryCode_nif},
    {"get_region_codes_for_country_calling_code", 1, GetRegionCodesForCountryCallingCode_nif},
    {"is_nanpa_country", 1, IsNANPACountry_nif},
    {"get_ndd_prefix_for_region", 2, GetNddPrefixForRegion_nif},
    {"is_possible_number_with_reason", 1, IsPossibleNumberWithReason_nif},
    {"is_possible_number", 1, IsPossibleNumber_nif},
    {"is_possible_number_for_string", 2, IsPossibleNumberForString_nif},
    {"get_example_number", 1, GetExampleNumber_nif},
    {"get_example_number_for_type", 2, GetExampleNumberForType_nif},
    {"get_example_number_for_non_geo_entity", 1, GetExampleNumberForNonGeoEntity_nif},
    {"parse", 2, Parse_nif},
    {"parse_and_keep_raw_input", 2, ParseAndKeepRawInput_nif},
    {"is_number_match", 2, IsNumberMatch_nif},
    {"is_number_match_with_two_strings", 2, IsNumberMatchWithTwoStrings_nif},
    {"is_number_match_with_one_string", 2, IsNumberMatchWithOneString_nif}
};

ERL_NIF_INIT(phonenumber_util, nif_funcs, on_nif_load, NULL, NULL, NULL)
