open System.IO

let findFirstMarker (s: string) (n: int) : int =
    let differentCharacters = Set.ofList >> Set.toList >> List.length

    let rec findFirstMarker' cs i n len =
        if i + n >= len then
            failwith "Could not find marker"
        else
            let count = cs |> List.take n |> differentCharacters

            if count = n then
                i + n
            else
                findFirstMarker' (List.tail cs) (i + 1) n len

    findFirstMarker' (s |> Seq.toList) 0 n s.Length

// 7
findFirstMarker "mjqjpqmgbljsphdztnvjfqwrcgsmlb" 4
// 5
findFirstMarker "bvwbjplbgvbhsrlpgdmjqwftvncz" 4
// 6
findFirstMarker "nppdvjthqldpwncqszvftbrmjlhg" 4
// 10
findFirstMarker "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg" 4
// 11
findFirstMarker "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw" 4

let input = "input.txt" |> File.ReadAllText
// 1912
findFirstMarker input 4

// 2122
findFirstMarker input 14
