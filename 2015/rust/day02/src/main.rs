use std::fs::read_to_string;

fn parse(s: &str) -> (i32, i32, i32) {
    let mut v: Vec<_> = s.split('x').map(|s| s.parse::<i32>().unwrap()).collect();
    v.sort_unstable();
    (v[0], v[1], v[2])
}

fn problem1(input: &Vec<(i32, i32, i32)>) -> i32 {
    input.iter().fold(0, |acc, (l, w, h)| {
        acc + 2 * l * w + 2 * w * h + 2 * h * l + l * w
    })
}

fn problem2(input: &Vec<(i32, i32, i32)>) -> i32 {
    input
        .iter()
        .fold(0, |acc, (l, w, h)| acc + l * w * h + 2 * l + 2 * w)
}

fn main() {
    let input: Vec<_> = read_to_string("../../input/day02.txt")
        .unwrap()
        .lines()
        .map(|s| parse(s))
        .collect();

    let ret1 = problem1(&input);
    assert_eq!(ret1, 1598415);

    let ret2 = problem2(&input);
    assert_eq!(ret2, 3812909);

    println!("ret1={ret1}");
    println!("ret2={ret2}");
}
