#include "info.h"
#include "core.h"
#include "EmbedMessage.h"
#include "JsonHandle.h"
#include "request_data.h"

using std::string;
using std::cout;
using std::vector;

void info_init(SaoFU::Client& bot) {
    bot.add_command(u8"info##獲取機器人資訊", [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        EmbedMessage embed(u8"哨夫", u8"使用|help 觀看說明");
        embed.color = 0x6be43f;
        embed.set_author(u8"bot 自我介紹",
            "https://ubisoft-avatars.akamaized.net/a1d253d8-3466-47c8-b8c8-2deace9d24e3/default_146_146.png?appId=39baebad-39e5-4552-8c25-2c9b919064e2");
        embed.set_thumbnail(
            "https://cdn.discordapp.com/avatars/733603376402661448/73e489ea96bcd64c9222f342382dd6ff.webp");
        embed.add_field(u8":white_check_mark:創建日期", u8"2021年8月6日", true);
        embed.add_field(u8":watch: bot啟動時間", SaoFU::get_time("now"), true);
        embed.add_field(u8"頭像來源", u8"JEFF", true);
        embed.add_field(u8"名稱來源", u8"jeff", true);
        embed.add_field(u8"說明", u8"您好，我是哨夫\n我使用c++語言寫成的機器人", false);
        bot.command->sendMessage(ctx.channelID, u8" ", embed);
    });

    bot.add_command(u8"help##顯示所有指令列表", [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        if (argv[1].empty()) {
            string content = u8"```所有指令列表\n\n";
            std::ostringstream ifs;
            for (auto& row : bot.cmd_info) {
                ifs << std::left << std::setw(20) << row.first << std::left << std::setw(20) << row.second << "\n";
            }
            content += ifs.str();
            content += u8"\n使用\\help [cmd] 可以獲得該指令的詳細使用方法```";

            bot.command->sendMessage(ctx.channelID, content);
        }
        else {
            string content = "```";
            for (auto& row : SaoFU::enumerate(bot.more_info)) {
                if (row.second.first.find(argv[1]) != string::npos) {
                    content += std::to_string(row.first) + ". " + bot.cmd_info[row.second.first] + u8"\n└──" + row.
                        second.second + "\n";
                }
            }
            content += "```";
            bot.command->sendMessage(ctx.channelID, content != "``````" ? content : u8"None");
        }
    });

    auto func_data = SaoFU::data_pack("func", u8"實驗功能")
                     .set_more_help(u8"Hide")
                     .set_argc(SaoFU::param_variadic(1));

    bot.add_command(func_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        if (ctx.author.ID.number() == 428201185724334092) {
            if (argv[1] == u8"json") {
                SaoFU::JsonHandle json_handle(bot.get_path[u8"redstone_HDMI"]);
                SaoFU::UrlHandle url_easy_handle(bot.get_path[u8"redstone_HDMI_url"]);
                url_easy_handle.urlpost(json_handle.str);
            }
            else if (argv[1] == u8"shutdown") {
                system("shutdown -s -t 60");
            }
            bot.command->sendMessage(ctx.channelID, "OK!");
            return;
        }
        bot.command->sendMessage(ctx.channelID, u8"好~");
    });
}
