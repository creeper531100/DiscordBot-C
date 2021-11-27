#ifndef _CLIENT
#include "sleepy_discord/sleepy_discord.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <Windows.h>
#include "core.h"
#include "JsonHandle.h"

#define _CLIENT

using Json = nlohmann::json;

namespace SaoFU {

    struct SettingDataPack {
        std::string cmd;
        std::string argc;
        std::string emoji;
        std::string help;
        std::string more_help;

        SettingDataPack() :
            cmd(u8"None"),
            argc(u8"-1000"),
            emoji(u8"None"),
            help(u8"�ȵL½Ķ"),
            more_help(u8"�ȵL½Ķ") {
        }

        static SettingDataPack create() {
            return SettingDataPack();
        }

        SettingDataPack& set_cmd(std::string data) {
            cmd = data;
            return *this;
        }

        SettingDataPack& set_argc(std::string data) {
            argc = data;
            return *this;
        }

        SettingDataPack& set_emoji(std::string data) {
            emoji = data;
            return *this;
        }

        SettingDataPack& set_help(std::string data) {
            help = data;
            return *this;
        }

        SettingDataPack& set_more_help(std::string data) {
            more_help = data;
            return *this;
        }
    };

    SettingDataPack data_pack(std::string cmd, std::string help = u8"�ȵL½Ķ");
    using SettingData = std::map<std::string, std::string>;
    using DataPack = std::function<void(SleepyDiscord::Message, std::vector<std::string>)>;
    using Message = SleepyDiscord::Message;

    class Client : public SleepyDiscord::DiscordClient {
    public:
        using SleepyDiscord::DiscordClient::DiscordClient;
        
        void command_prefix(std::string);
        void set_json_path(nlohmann::json);
        void onMessage(Message) override;
        void onReaction(SleepyDiscord::Snowflake<SleepyDiscord::User>, SleepyDiscord::Snowflake<SleepyDiscord::Channel>,
                        SleepyDiscord::Snowflake<SleepyDiscord::Message>, SleepyDiscord::Emoji) override;
        void add_command(std::string, DataPack);
        void add_command(SettingDataPack&, DataPack);
        bool is_emoji;

        nlohmann::json get_path;
        SettingData cmd_info;
        SettingData more_info;
        Client* command;
        std::string prefix;
    private:
        std::unordered_map<std::string, std::string> emoji_map;
        std::map<std::string, int> argc;
        std::unordered_map<std::string, DataPack> m_commands;
        std::vector<std::string> help_file_data;
    };
}
#endif
