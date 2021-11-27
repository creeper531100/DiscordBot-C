#include "userfileadd.h"
#include "JsonHandle.h"
#include "core.h"

using std::vector;
using std::cout;
using std::string;

void userfileadd_init(SaoFU::Client& bot) {
    auto addcbn_data = SaoFU::data_pack("addcbn", u8"�s�W�զX�y")
                       .set_more_help(u8"open_file")
                       .set_argc(SaoFU::param_variadic(2));

    bot.add_command(addcbn_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        SaoFU::UrlHandle post(bot.get_path[u8"redstone_HDMI_url"]);
        auto json_handle = SaoFU::JsonHandle::parse(post.urlopen().data);

        if (json_handle.is_json) {
            vector<string> sentence_element;
            SaoFU::transform_item(json_handle[u8"�欰"].json(), sentence_element);

            if (SaoFU::find(argv[1]).in(sentence_element).is_true)
                json_handle[u8"�欰"][argv[1]].append(argv[2]);
            else
                json_handle[u8"�欰"].updata(argv[1], vector<string>{ argv[2] });

            post.urlpost(json_handle.json().dump(4));
            json_handle.output(bot.get_path[u8"redstone_HDMI"]);

            bot.command->sendMessage(ctx.channelID, u8"���\�ɤJ");
        }
        else
            bot.command->sendMessage(ctx.channelID, u8"���ɤJ");
    });

    auto addrand_data = SaoFU::data_pack("addrand", u8"�s�W����")
                        .set_more_help(u8"|addrand [�s�W�y�y]")
                        .set_argc(SaoFU::param_variadic(1));

    bot.add_command(addrand_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        SaoFU::UrlHandle post(bot.get_path[u8"redstone_HDMI_url"]);
        auto json_handle = SaoFU::JsonHandle::parse(post.urlopen().data);
        if (json_handle.is_json) {
            json_handle[u8"���y"].append(argv[1]);
            bot.command->sendMessage(ctx.channelID, u8"���\�ɤJ");
            post.urlpost(json_handle.json().dump(4));
            json_handle.output(bot.get_path[u8"redstone_HDMI"]);
        }
        else
            bot.command->sendMessage(ctx.channelID, u8"���ɤJ");
    });
}
