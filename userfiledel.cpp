#include "userfiledel.h"
#include "JsonHandle.h"
#include "core.h"

using std::vector;
using std::cout;
using std::string;

class DelHandle {
public:
    Json json;

    DelHandle(Json path) : json(path) {
    }

    bool handle(string keyword, vector<string> argv, std::function<void(SaoFU::JsonHandle&, int)> func) {
        SaoFU::UrlHandle post(json[u8"redstone_HDMI_url"]);
        auto json_handle = SaoFU::JsonHandle::parse(post.urlopen().data);

        if (json_handle.is_json) {
            vector<string> word_list = json_handle[keyword].json();
            auto sentence_obj = SaoFU::find<SaoFU::SEARCH_CONTENT>(argv[1]).in(word_list);
            if (sentence_obj.is_true) {
                func(json_handle, sentence_obj.index);
                post.urlpost(json_handle.json().dump(4));
                json_handle.output(json[u8"redstone_HDMI"]);
                return 1;
            }
        }
        return 0;
    }
};

void userfiledel_init(SaoFU::Client& bot) {
    DelHandle* del_handle = new DelHandle(bot.get_path);

    auto delrand_data = SaoFU::data_pack("delrand", u8"刪除屁話")
                        .set_more_help(u8"|delrand [刪除語句]")
                        .set_argc(SaoFU::param_variadic(1));

    bot.add_command(delrand_data, [&bot, del_handle](SleepyDiscord::Message ctx, vector<string> argv) {
        auto successful = del_handle->handle(u8"詞語", argv, [&](SaoFU::JsonHandle& json_handle, int pos) {
            string str = json_handle[u8"詞語"][pos].json();
            json_handle[u8"詞語"].remove(pos);
            json_handle[u8"刪除"].append(str);
        });
        if (successful)
            bot.command->sendMessage(ctx.channelID, u8"成功刪除");
        else
            bot.command->sendMessage(ctx.channelID, u8"未刪除");
    });

    auto recrand_data = SaoFU::data_pack("recrand", u8"復原屁話")
                        .set_more_help(u8"|recrand [復原語句]")
                        .set_argc(SaoFU::param_variadic(1));

    bot.add_command(recrand_data, [&bot, del_handle](SleepyDiscord::Message ctx, vector<string> argv) {
        auto successful = del_handle->handle(u8"刪除", argv, [&](SaoFU::JsonHandle& json_handle, int pos) {
            string str = json_handle[u8"刪除"][pos].json();
            json_handle[u8"刪除"].remove(pos);
            json_handle[u8"詞語"].append(str);
        });
        if (successful)
            bot.command->sendMessage(ctx.channelID, u8"成功復原");
        else
            bot.command->sendMessage(ctx.channelID, u8"未復原");
    });
}
