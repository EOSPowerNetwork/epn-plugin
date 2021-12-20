#include <eosio/chain_plugin/chain_plugin.hpp>
#include <eosio/epn_plugin/epn_plugin.hpp>
#include <string>

namespace eosio {
static appbase::abstract_plugin &_epn_plugin =
    app().register_plugin<epn_plugin>();

class epn_plugin_impl {
public:
  void on_block_start(const uint32_t &block_num) {
    // ilog("EOS Power Network plugin:  on_block_start() begin, block number: "
    //      "${blocknum}",
    //      ("blocknum", std::to_string(block_num)));
  }

  fc::optional<boost::signals2::scoped_connection> on_block_connection;
};

epn_plugin::epn_plugin() : my(new epn_plugin_impl()) {}
epn_plugin::~epn_plugin() {}

void epn_plugin::set_program_options(options_description &,
                                     options_description &cfg) {

  // cfg.add_options()
  //         ("option-name", bpo::value<string>()->default_value("default
  //         value"), "Option Description")
  //     ;
}

void epn_plugin::plugin_initialize(const variables_map &options) {
  ilog("EOS Power Network plugin:  plugin_initialize() begin");
  try {
    // if( options.count( "option-name" )) {
    // Handle the option
    //}
  }
  FC_LOG_AND_RETHROW()
}

void epn_plugin::plugin_startup() {
  ilog("EOS Power Network plugin:  plugin_startup() begin");
  try {
    auto &chain = app().find_plugin<chain_plugin>()->chain();
    my->on_block_connection.emplace(chain.block_start.connect(
        [&](const uint32_t &block_num) { my->on_block_start(block_num); }));
  }
  FC_LOG_AND_DROP();
}

void epn_plugin::plugin_shutdown() {
  ilog("EOS Power Network plugin:  plugin_shutdown() begin");
  my->on_block_connection.reset();
}

} // namespace eosio
