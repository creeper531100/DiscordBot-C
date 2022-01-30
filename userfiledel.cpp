#include "userfiledel.h"
#include "JsonHandle.h"
#include "core.h"

using std::vector;
using std::cout;
using std::string;

void transfer_handle(SaoFU::Client& bot, SleepyDiscord::Message& ctx, vector<string>& argv, std::string remove_keyword, std::string append_keyword) {
    SaoFU::UrlHandle post(bot.get_path[u8"redstone_HDMI_url"]);
    auto json_handle = SaoFU::JsonHandle::parse(post.urlopen().data);

    if (json_handle.is_json) {
        vector<string> word_list = json_handle[remove_keyword].json();
        auto sentence_obj = SaoFU::find<SaoFU::SEARCH_CONTENT>(argv[1]).in(word_list);
        if (sentence_obj.is_true) {
            string str = json_handle[remove_keyword][sentence_obj.index].json();
            json_handle[remove_keyword].remove(sentence_obj.index);
            json_handle[append_keyword].append(str);
            post.urlpost(json_handle.json().dump(4));
            json_handle.output(bot.get_path[u8"redstone_HDMI"]);
            bot.command->sendMessage(ctx.channelID, u8"���\");
            return;
        }
    }
    bot.command->sendMessage(ctx.channelID, u8"�����\");
}

void userfiledel_init(SaoFU::Client& bot) {
    auto delrand_data = SaoFU::data_pack("delrand", u8"�R������")
                        .set_more_help(u8"|delrand [�R���y�y]")
                        .set_argc(SaoFU::param_variadic(1));

    bot.add_command(delrand_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        transfer_handle(bot, ctx, argv, u8"���y", u8"�R��");
    });

    auto recrand_data = SaoFU::data_pack("recrand", u8"�_�짾��")
                        .set_more_help(u8"|recrand [�_��y�y]")
                        .set_argc(SaoFU::param_variadic(1));

    bot.add_command(recrand_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        transfer_handle(bot, ctx, argv, u8"�R��", u8"���y");
    });
}
