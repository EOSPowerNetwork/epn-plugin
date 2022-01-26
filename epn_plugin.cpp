
#include <eosio/chain/abi_serializer.hpp>
#include <eosio/chain/action.hpp>
#include <eosio/chain_plugin/chain_plugin.hpp>
#include <eosio/epn_plugin/epn_plugin.hpp>
#include <fc/io/json.hpp>
#include <string>

namespace eosio
{
    static appbase::abstract_plugin& _epn_plugin = app().register_plugin<epn_plugin>();

    namespace epn_helpers
    {
        bool _validateAccount(const eosio::chain::controller& cc, eosio::chain::name accName) {
            auto* account_obj = cc.db().find<eosio::chain::account_object, eosio::chain::by_name>(accName);
            if (account_obj == nullptr) {
                elog("EPN transaction failed: Blockchain account ${accName} is missing.", ("accName", accName));
                return false;
            }
            return true;
        }
    };  // namespace epn_helpers

    class epn_plugin_impl {
       public:
        void setMaxActionsPerBlock(const uint64_t numActionsPerBlock) {
            _actionsPerBlock = numActionsPerBlock;
        }
        void setSigningPrivateKey(const fc::crypto::private_key& privKey) {
            _privateKey = privKey;
        }

        void setPermission(const std::string& permission) {
            _permission = permission;
        }

        void setOperatorName(const std::string& operatorName) {
            _operatorName = operatorName;
        }

        void on_block_start(const uint32_t& block_num) {
            for (uint64_t i = 0; i < _actionsPerBlock; ++i) {
                _sendAction(i);
            }
        }

        fc::optional<boost::signals2::scoped_connection> _connection;

       private:
        void _sendAction(const uint64_t& actionNum);
        bool _isValidAccount(eosio::chain::name);

        uint64_t _actionsPerBlock = 0;
        fc::crypto::private_key _privateKey;
        std::string _permission;
        std::string _operatorName;
    };

    void epn_plugin_impl::_sendAction(const uint64_t& actionNum) {
        using eosio::chain::account_object;
        using eosio::chain::action;
        using eosio::chain::by_name;
        using eosio::chain::mutable_variant_object;
        using eosio::chain::name;
        using eosio::chain::packed_transaction;
        using eosio::chain::permission_level;
        using eosio::chain::signed_transaction;
        using eosio::chain::transaction_trace_ptr;

        auto& chain = app().get_plugin<chain_plugin>();

        controller& cc = chain.chain();
        name execContract = N("exec.epn");
        if (!epn_helpers::_validateAccount(cc, execContract)) {
            return;
        }
    };

    epn_plugin::epn_plugin()
        : my(new epn_plugin_impl()) {}
    epn_plugin::~epn_plugin() {}

    void epn_plugin::set_program_options(options_description&, options_description& cfg) {
        // clang-format off
        cfg.add_options()
            ("max-actions-per-block", bpo::value<uint64_t>()->default_value(2), "Max number of EPN actions executed per block")
            ("signing-private-key", bpo::value<string>()->default_value("PRIVATE_KEY")->required(), "Singing private key")
            ("permission", bpo::value<string>()->default_value("active"), "The permission used to sign the EPN transactions")
            ("operator-name", bpo::value<string>()->required(), "Name of the operator executing EPN actions")
            ;
        // clang-format on
    }

    void epn_plugin::plugin_initialize(const variables_map& options) {
        try {
            auto handleOption = [&](const std::string& teststring, auto func) {
                if (options.count(teststring)) {
                    func(options.at(teststring));
                }
            };

            // clang-format off
            handleOption("max-actions-per-block", [&](const boost::program_options::variable_value& value) {
                ilog("Converting max actions per block");
                my->setMaxActionsPerBlock(value.as<uint64_t>()); 
            });

            handleOption("signing-private-key", [&](const boost::program_options::variable_value& value) {
                ilog("Converting signing private key");
                my->setSigningPrivateKey(fc::crypto::private_key(value.as<std::string>()));
            });

            handleOption("permission", [&](const boost::program_options::variable_value& value) {
                ilog("Converting permission");
                my->setPermission(value.as<std::string>());
            });

            handleOption("operator-name", [&](const boost::program_options::variable_value& value) {
                ilog("Converting operator name");
                my->setOperatorName(value.as<std::string>()); 
            });
            // clang-format on
        }
        FC_LOG_AND_RETHROW()
    }

    void epn_plugin::plugin_startup() {
        try {
            auto& chain = app().find_plugin<chain_plugin>()->chain();

            my->_connection.emplace(chain.block_start.connect([&](const uint32_t& block_num) { my->on_block_start(block_num); }));

            /*
                signal<void(uint32_t)>                        block_start; //
                block_num signal<void(const signed_block_ptr&)> pre_accepted_block;
                signal<void(const block_state_ptr&)> accepted_block_header;
                signal<void(const block_state_ptr&)>          accepted_block;
                signal<void(const block_state_ptr&)>          irreversible_block;
                signal<void(const transaction_metadata_ptr&)> accepted_transaction;
                signal<void(std::tuple<const transaction_trace_ptr&, const
                signed_transaction&>)> applied_transaction;
            */
        }
        FC_LOG_AND_DROP();
    }

    void epn_plugin::plugin_shutdown() {
        my->_connection.reset();
    }

}  // namespace eosio
