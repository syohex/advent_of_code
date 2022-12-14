open System.IO

let findFirstMarker (s: string) (n: int) : int =
    s
    |> Seq.windowed n
    |> Seq.indexed
    |> Seq.find (fun (_, cs) -> cs |> Seq.distinct |> (fun ds -> Seq.length ds = n))
    |> fst
    |> (fun index -> index + n)

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
