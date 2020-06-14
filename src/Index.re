open Serbet.Endpoint;
open Async;
open Globals;

Dotenv.config();
[@bs.val] external port: string = "process.env.PORT";

module HomeAnimals = {
  [@decco.decode]
  type body_in = unit;
  [@decco.encode]
  type homeAnimals = {id: string};
  [@decco.encode]
  type body_out = array(homeAnimals);
  let endpoint =
    Serbet.jsonEndpoint({
      verb: POST,
      path: "/homeAnimals",
      body_in_decode,
      body_out_encode,
      handler: (_body, _req) => {
        // TODO: add logic that chooses which animals to display on the home page.
        [|
          {id: "1"},
          {id: "2"},
          {id: "6"},
          {id: "8"},
          {id: "11"},
          {id: "42"},
        |]
        ->async;
      },
    });
};

let app =
  Serbet.application(
    ~port=port->int_of_string_opt |||| 9898,
    [HomeAnimals.endpoint],
  );
