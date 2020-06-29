open Serbet.Endpoint;
open Globals;

Dotenv.config();
[@bs.val] external port: string = "process.env.PORT";

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
        [|"1", "2", "6", "8", "11", "18", "21", "42"|]
        ->convertIdArrayIntoHomepageDetails
        ->async;
      },
    });
};

let app =
  Serbet.application(
    ~port=port->int_of_string_opt |||| 9898,
    [HomeAnimals.endpoint],
  );
