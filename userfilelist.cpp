#include "userfiledel.h"
#include "core.h"

void file_handle(SaoFU::Client& bot, SleepyDiscord::Message ctx, std::string keyword) {
    SaoFU::JsonHandle json(bot.get_path[u8"redstone_HDMI"]);
    json[keyword].output("DATA\\tmp.json");
    bot.command->uploadFile(ctx.channelID, u8"DATA\\tmp.json", u8"如圖所示");
}

void userfilelist_init(SaoFU::Client& bot) {
    auto cbnlist_data = SaoFU::data_pack("cbnlist", u8"組合詞列表").set_emoji(u8"🙏");

    bot.add_command(cbnlist_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        file_handle(bot, ctx, u8"行為");
    });

    bot.add_command(u8"randlist##詞語列表", [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        file_handle(bot, ctx, u8"詞語");
    });
}
