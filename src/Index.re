open Serbet.Endpoint;
open Globals;

Dotenv.config();
[@bs.val] external port: option(string) = "process.env.PORT";

module HomeAnimals = {
  [@decco.decode]
  type body_in = unit;
  [@decco.encode]
  type homeAnimals = {
    id: string,
    prev: string,
    next: string,
  };
  [@decco.encode]
  type body_out = array(homeAnimals);
  let convertIdArrayIntoHomepageDetails: array(string) => body_out =
    animalArray => {
      let length = animalArray->Array.length;

      Array.mapWithIndex(
        animalArray,
        (index, item) => {
          Js.log3(
            (length + index - 1) mod length,
            " - ",
            (index + 1) mod length,
          );
          {
            id: item,
            prev:
              animalArray->Array.getUnsafe((length + index - 1) mod length),
            next: animalArray->Array.getUnsafe((index + 1) mod length),
          };
        },
      );
    };
  let endpoint =
    Serbet.jsonEndpoint({
      verb: POST,
      path: "/homeAnimals",
      body_in_decode,
      body_out_encode,
      handler: (_body, _req) => {
        // TODO: add logic that chooses which animals to display on the home page.
        [|"1", "42", "2", "23", "11", "18", "21", "6"|]
        ->convertIdArrayIntoHomepageDetails
        ->async;
      },
    });
};

    // let transactionListener = contract.methods
    //   .executeMetaTransaction(userAddress, functionSignature, r, s, v)
    //   .send({
    //     from: userAddress,
    //   });
  [@decco.decode]
  type metaTxInfo = {userAddress: string, functionSignature: string, r: string, s: string, v: int};

[@decco.encode]
type body_out = {success: bool, txHash: string};

[@bs.module "./executeTransaction"]
external execMetaTx:
  (
    . metaTxInfo
  ) =>
  Js.Promise.t(body_out) =
  "execMetaTx";

module MetaTx = {
  [@decco.decode]
  type body_in = {input: metaTxInfo};

  let endpoint =
    Serbet.jsonEndpoint({
      verb: POST,
      path: "/metaTx",
      body_in_decode,
      body_out_encode,
      handler: ({input}, _req) => {
        execMetaTx(. input);
      },
    });
};
  
/*
module Auth = {
  type authResponse = {
    [@bs.as "X-Hasura-User-Id"]
    xHasuraUserId: option(string),
    [@bs.as "X-Hasura-Role"]
    xHasuraRole: string,
    // [@bs.as "X-Hasura-Is-Owner"]
    // xHasuraIsOwner: string,
    // [@bs.as "X-Hasura-Custom"]
    // xHasuraCustom: string,
  };
  type headers = {
    [@bs.as "eth-signature"]
    ethSignature: option(string),
    [@bs.as "eth-address"]
    ethAddress: option(string),
    [@bs.as "jwt-token"]
    auth: option(string),
    [@bs.as "eth-login-jwt"]
    ethJwt: option(string),
  };
  type authInput = {headers};

  let validateEthSignature = (ethSignature, ethAddress) => {
    let web3 = Web3.make();
    let result =
      Web3.ecRecover(
        web3,
        "signin-string-for-wildcards:" ++ ethAddress,
        ethSignature,
      );
    result == ethAddress;
  };

  // NOT typesafe, but there is a level of trust from hasura :)
  let getAuthHeaders: Express.Request.t => authInput = [%raw
    {|(req) => req.body|}
  ];
  let getJwt: Express.Request.t => string = [%raw {|(req) => req.jwt|}];

  let authResponseToJson: authResponse => Js.Json.t = Obj.magic;

  let endpoint =
    Serbet.endpoint({
      verb: POST,
      path: "/auth",
      handler: req => {
        let {headers: {ethSignature, ethAddress, ethJwt, auth}} =
          getAuthHeaders(req.req);
        let jwt: string = getJwt(req.req);
        Js.log2("The awaited jwt!!", jwt);
        switch (ethSignature, ethAddress, auth) {
        | (Some(ethSignature), Some(ethAddress), None, _) =>
          if (validateEthSignature(ethSignature, ethAddress)) {
            {xHasuraUserId: Some(ethAddress), xHasuraRole: "organisation"}
            ->authResponseToJson
            ->OkJson
            ->async;
          } else {
            Unauthorized("")->async;
          }
        | (Some(ethSignature), Some(ethAddress), Some(ethJwt), _) =>
          // TODO: validate the jwt here - create users email
          if (validateEthSignature(ethSignature, ethAddress)) {
            {xHasuraUserId: Some(ethAddress), xHasuraRole: "organisation"}
            ->authResponseToJson
            ->OkJson
            ->async;
          } else {
            Unauthorized("")->async;
          }
        | (_, _, _, Some(jwtAuth)) =>
          Js.log2("TODO: Implement jwt auth!", jwtAuth);

          Js.log2("WE HAVE THE JWT!!", jwt);
          {xHasuraUserId: None, xHasuraRole: "public"}
          ->authResponseToJson
          ->OkJson
          ->async;
        | _ =>
          {xHasuraUserId: None, xHasuraRole: "public"}
          ->authResponseToJson
          ->OkJson
          ->async
        };
      },
    });
};
*/

let app =
  Serbet.application(
    ~port=(port |||| "9898")->int_of_string_opt |||| 9898,
    [HomeAnimals.endpoint, MetaTx.endpoint],
  );
