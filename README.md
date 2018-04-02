elibphonenumber
===============

Erlang NIF for [libphonenumber](https://github.com/googlei18n/libphonenumber).

## Note

- This project is active. I'm doing updates often because I see the original author is no longer maintaining it.
- This repository is a fork of [silviucpp/elibphonenumber][3], except this version is designed to be used with rebar3 & hex.pm,
  which means we have elixir support.

## Dependencies

You need compile `libphonenumber` and install it before compiling the project. You can read up on how people have done this on the [wiki.][2]

## Use with mix

```elixir
defp deps do
  [{:elibphonenumber, "~> 1.0"}]
end
```

## Run the tests

```sh
rebar3 compile
rebar3 eunit
```

[1]:https://www.wowapp.com/w/silviu/Silviu-Caragea
[2]:https://github.com/johnhamelink/elibphonenumber/wiki/Compiling-Libphonenumber
[3]:https://github.com/silviucpp/elibphonenumber
