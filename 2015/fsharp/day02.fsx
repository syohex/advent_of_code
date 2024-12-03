open System.IO

type Data =
    { Length: int; Width: int; Height: int }

let Parse (s: string) : Data =
    s.Split('x')
    |> Array.map int
    |> fun a ->
        { Length = a.[0]
          Width = a.[1]
          Height = a.[2] }

let SmallestArea (d: Data) : int =
    let area1, area2, area3 =
        d.Length * d.Width, d.Width * d.Height, d.Height * d.Length

    min area1 (min area2 area3)

let SquareFeet (d: Data) : int =
    2 * d.Length * d.Width + 2 * d.Width * d.Height + 2 * d.Height * d.Length

let Problem1 (input: Data list) : int =
    input |> List.fold (fun acc d -> acc + SquareFeet d + SmallestArea d) 0

let ShortestPerimeter (d: Data) : int =
    let len1, len2 =
        [ d.Length; d.Width; d.Height ]
        |> List.sort
        |> fun v -> List.head v, List.item 1 v

    2 * len1 + 2 * len2

let Problem2 (input: Data list) : int =
    input
    |> List.fold (fun acc d -> acc + ShortestPerimeter d + d.Length * d.Width * d.Height) 0

// 58
Problem1 [ Parse "2x3x4" ]

// 43
Problem1 [ Parse "1x1x10" ]

// 34
Problem2 [ Parse "2x3x4" ]
// 14
Problem2 [ Parse "1x1x10" ]

let input = File.ReadLines("../input/day02.txt") |> Seq.map Parse |> Seq.toList
// 1598414
Problem1 input

// 3812909
Problem2 input
