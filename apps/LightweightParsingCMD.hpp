/// Source: https://www.codeproject.com/Tips/5261900/Cplusplus-Lightweight-Parsing-Command-Line-Argumen
/// \author Daniel Petrovic
/// \date 13 March 2020

#include <functional>   // std::function
#include <iostream>     // std::cout, std::endl
#include <map>          // std::map
#include <memory>       // std::unique_ptr
#include <string>       // std::string
#include <sstream>      // std::stringstream
#include <string_view>  // std::string_view
#include <variant>      // std::variant
#include <vector>       // std::vector
using namespace std;

template <class Opts>
struct CmdOpts : Opts
{
    using MyProp = std::variant<string Opts::*, int Opts::*, bool Opts::*, double Opts::*,
                                uint32_t Opts::*, uint8_t Opts::*>;
    using MyArg = std::pair<string, MyProp>;

    ~CmdOpts() = default;

    Opts parse(int argc, const char* argv[])
    {
        vector<string_view> vargv(argv, argv+argc);
        for (int idx = 0; idx < argc; ++idx)
            for (auto& cbk : callbacks)
                cbk.second(idx, vargv);

        return static_cast<Opts>(*this);
    }

    static unique_ptr<CmdOpts> Create(std::initializer_list<MyArg> args)
    {
        auto cmdOpts = unique_ptr<CmdOpts>(new CmdOpts());
        for (auto arg : args) cmdOpts->register_callback(arg);
        return cmdOpts;
    }

private:
    using callback_t = function<void(int, const vector<string_view>&)>;
    map<string, callback_t> callbacks;

    CmdOpts() = default;
    CmdOpts(const CmdOpts&) = delete;
    CmdOpts(CmdOpts&&) = delete;
    CmdOpts& operator=(const CmdOpts&) = delete;
    CmdOpts& operator=(CmdOpts&&) = delete;

    auto register_callback(string name, MyProp prop)
    {
        callbacks[name] = [this, name, prop](int idx, const vector<string_view>& argv)
        {
            if (argv[idx] == name)
            {
                visit(
                    [this, idx, &argv](auto&& arg)
                    {
                        if (idx < argv.size() - 1)
                        {
                            if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, bool Opts::*>)
                                this->*arg = true;
                            else
                            {
                                stringstream value;
                                value << argv[idx+1];
                                value >> this->*arg;
                            }
                        }
                    },
                    prop);
            }
        };
    };

    auto register_callback(MyArg p) { return register_callback(p.first, p.second); }
};
