#include "Client.h"
#include "info.h"
#include "userfile.h"
#include "crawler.h"
#include "JsonHandle.h"
#include "userfileadd.h"
#include "userfiledel.h"
using namespace std;

int main() {
    SaoFU::JsonHandle data("D:\\data.json");
    SaoFU::Client bot(data[u8"token"].json(), SleepyDiscord::USER_CONTROLED_THREADS);
    bot.command_prefix("\\");
    bot.set_json_path(data.json());

    userfile_init(bot);
    info_init(bot);
    crawler_init(bot);
    userfileadd_init(bot);
    userfiledel_init(bot);

    bot.run();
    return 0;
}
