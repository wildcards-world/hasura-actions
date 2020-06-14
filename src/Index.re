open Serbet.Endpoint;
open Async;
open Globals;

Dotenv.config();
[@bs.val] external port: string = "process.env.PORT";

module HomeAnimals = {
  [@decco.encode]
  type homeAnimals = {id: string};
  [@decco.encode]
  type homeAnimalsList = array(homeAnimals);
  let endpoint =
    Serbet.endpoint({
      verb: POST,
      path: "/homeAnimals",
      handler: req => {
        // let%Async params = req.requireParams(tokenUriParams_decode);
        // TODO: add logic that chooses which animals to display on the home page.
        OkJson(
          [|
            {id: "1"},
            {id: "2"},
            {id: "6"},
            {id: "8"},
            {id: "11"},
            {id: "42"},
          |]
          ->homeAnimalsList_encode,
        )
        ->async;
      },
    });
};

let app =
  Serbet.application(
    ~port=port->int_of_string_opt |||| 9898,
    [HomeAnimals.endpoint],
  );
