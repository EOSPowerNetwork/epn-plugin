# EPN Plugin
This plugin is used by the EOS Power Network operators to facilitate pull transactions. The EPN Operators are EOS block producers (or BP candidates) who already have block producing infrastructure set up. This plugin can be installed into the node clients to allow them to also service pull transactions (only while they are not also the active producer on the EOS blockchain to help performance).

## How does it work?
When someone creates a pull transaction on the EOS Power Network, the transaction goes into a queue ordered by the block height at which the transaction must be executed. This plugin checks the queue and executes the n top transactions (n is set according to whatever parameter strikes the best balance between block producer performance and EPN operator performance. More details can be found [here](https://docs.eospower.network/design/cron#maximum-number-of-serviceable-contracts).

There is **no** guarantee that the block height mapped to a transaction will be the block height at which a transaction is executed. The transactions are simply executed in order of blockheight, and will not be executed *before* that block height is reached.

## Who can use this?
Facilitating a pull transaction can actually be done by anyone, this plugin is simply an easy way to allow power network operators to be the ones to execute the transactions. If someone develops a better tool to automate the execution of the EPN pull transactions (If, perhaps, BPs are not interested in adding this plugin to their client), that's fine. Even non-block-producers could execute these transactions, although they will be paying for the resource costs of executing the transaction and should not expect to be compensated by the EPN. 

# Conclusion
This is a very simple, but powerful plugin. It acknowledges the reality that block producers are a set of entities hired by a decentralized network to perform a service, namely, block production - but there is no reason that other services could not also be paid for by the same decentralized public network. More information about the EPN, its design, intent, and philosophy, can be found at eospower.network or by reading the [whitepaper](https://eospower.network/files/EPN-Whitepaper.pdf).

