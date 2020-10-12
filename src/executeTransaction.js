const Web3 = require("web3");
const HDWalletProvider = require("@truffle/hdwallet-provider");
const dotenv = require("dotenv");
const toBuffer = require("ethereumjs-util").toBuffer;
const abi = require("ethereumjs-abi");
const events = require("events");
var Contract = require("web3-eth-contract");
const jsonInterface = require("./WildcardSteward_v3_matic.json");
const contractAddress = "0x59b3c176c39bd8734717492f4da8fe26ff6a454d";

const execMetaTx = ({userAddress, functionSignature, r, s, v}) => {
  dotenv.config();

  const mnemonicPhrase =
    process.env.MNEMONIC_PHRASE ||
    "rookie dolphin castle lawsuit spawn kingdom alone cabbage below invest insane scissors";
  const providerUrl = process.env.PROVIDER_URL || "http://localhost:8545";

  provider = new HDWalletProvider({
    mnemonic: {
      phrase: mnemonicPhrase,
    },
    providerOrUrl: providerUrl,
  });

  const web3 = new Web3(provider)

  var contract = new web3.eth.Contract(
    jsonInterface.abi,
    contractAddress
  );

  // No need to calculate gas limit or gas price here
  let transactionListener = contract.methods
    .executeMetaTransaction(userAddress, functionSignature, r, s, v)
    .send({ from: "0x8b1215771475Cf3EA4d548575e7EA23ba955e8Ed"})

  return new Promise((res, rej) => transactionListener
    .on("transactionHash", (hash) => {
      res({ success: true, txHash: hash})
    })
    // .once("confirmation", (confirmation, recipet) => {
    //   eventEmitter.emit("confirmation", confirmation, recipet);
    // })
    .on("error", (error) => {
      rej(error)
    }));
}

module.exports = {execMetaTx};
