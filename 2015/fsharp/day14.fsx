open System.Text.RegularExpressions

type Data =
    { Name: string
      Velocity: int
      FlyingSeconds: int
      RestingSeconds: int }

let parse (input: string list) : Data list =
    let re =
        new Regex(@"(\S+) can fly (\S+) km/s for (\S+) seconds, but then must rest for (\S+) seconds.")

    input
    |> List.map (fun s ->
        let m = re.Match(s)

        if m.Success then
            { Name = m.Groups.[1].Value
              Velocity = int m.Groups.[2].Value
              FlyingSeconds = int m.Groups.[3].Value
              RestingSeconds = int m.Groups.[4].Value }
        else
            failwithf "invalid input %s" s)

let testInput =
    [ "Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds."
      "Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds." ]

parse testInput
