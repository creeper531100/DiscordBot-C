#include "Client.h"
#include "core.h"
#include <algorithm>

using std::vector;
using std::cout;
using std::string;


SaoFU::SettingDataPack SaoFU::data_pack(std::string cmd, std::string help) {
    return SettingDataPack::create()
           .set_cmd(cmd)
           .set_help(help);
}

void SaoFU::Client::command_prefix(string prefix) {
    this->prefix = prefix;
    command = this;
    is_emoji = false;
    std::ifstream ifs("DATA\\readme.txt");
    std::string help_file((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    this->help_file_data = SaoFU::string_split(help_file, u8"-*-");
}

void SaoFU::Client::set_json_path(nlohmann::json json) {
    this->get_path = json;
}

void SaoFU::Client::onMessage(Message message) {
    try {
        if (message.startsWith(prefix) && !message.author.bot) {
            vector<string> argv = SaoFU::string_split(message.content, " ");
            string cmd = argv[0];

            if (m_commands.find(cmd) != m_commands.end()) {
                int arg_count = argc[cmd];
                bool is_unspecified = false;

                if (arg_count < 0) {
                    arg_count = abs(arg_count);

                    if (arg_count >= 1000) {
                        arg_count /= 1000;
                        is_unspecified = true;
                    }

                    if (argv.size() < arg_count) {
                        sendMessage(message.channelID, u8"參數太少");
                        return;
                    }

                    string data = "";
                    int index = 0;
                    std::for_each(argv.begin() + arg_count, argv.end(), [&data, &index](auto it) {
                        data += (index++) == 0 ? it : " " + it;
                    });

                    argv.erase(argv.begin() + arg_count, argv.end());

                    /*
                     * 假如data是空的
                     * 進入條件
                     * 判斷是否有設定推空格的條件
                     */

                    if (!data.empty() || is_unspecified) {
                        argv.push_back(data);
                    }

                }

                if (arg_count + 1 == argv.size()) {
                    m_commands[cmd](message, argv);
                }
                else {
                    sendMessage(message.channelID, u8"參數錯誤");
                }

            }
            else {
                sendMessage(message.channelID, u8"沒有此指令");
            }
        }
    }
    catch (SleepyDiscord::ErrorCode e) {
        cout << e << std::endl;
    }
}

void SaoFU::Client::onReaction(SleepyDiscord::Snowflake<SleepyDiscord::User> userID,
                               SleepyDiscord::Snowflake<SleepyDiscord::Channel> channelID,
                               SleepyDiscord::Snowflake<SleepyDiscord::Message> messageID, SleepyDiscord::Emoji emoji) {
    if (m_commands.find(emoji_map[emoji.name]) != m_commands.end()) {
        is_emoji = true;
        this->m_commands[emoji_map[emoji.name]](getMessage(channelID, messageID),
            vector<string>{SleepyDiscord::User(getUser(userID)).username});
    }
    is_emoji = false;
}

void SaoFU::Client::add_command(string name, DataPack callback) {
    vector<string> cmd_list = SaoFU::string_split(name, "##");
    if (cmd_list.size() > 1) {
        if (cmd_list[1] != u8"Hide")
            cmd_info[cmd_list[0]] = cmd_list[1];
    }
    else
        cmd_info[cmd_list[0]] = u8"暫無翻譯";
    string cmd = prefix + cmd_list[0];
    this->argc[cmd] = -1000;
    m_commands[cmd] = callback;
}

void SaoFU::Client::add_command(SettingDataPack& data, DataPack callback) {
    if (data.cmd == u8"None") {
        cout << "無法建立" << data.cmd << std::endl;
        return;
    }

    string cmd = prefix + data.cmd;

    if (!find("Hide").in(data.more_help).is_true)
        cmd_info[data.cmd] = data.help;

    if (find("open_file").in(data.more_help).is_true) {
        auto cmd_obj = find(data.cmd).in(help_file_data);
        if (cmd_obj.is_true) {
            int pos = cmd_obj.index;
            more_info[data.cmd] = help_file_data[pos + 1];
        }
    }
    else
        more_info[data.cmd] = data.more_help;

    argc[cmd] = std::stoi(data.argc);
    m_commands[cmd] = callback;
    emoji_map[data.emoji] = cmd;
}
