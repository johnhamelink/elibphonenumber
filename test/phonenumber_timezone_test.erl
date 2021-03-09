-module(phonenumber_timezone_test).

-include_lib("eunit/include/eunit.hrl").

get_timezone_test() ->
    application:ensure_all_started(elibphonenumber),
    {ok,[<<"America/Los_Angeles">>]} = phonenumber_to_timezones:timezones_for_number(<<"16502530000">>).
