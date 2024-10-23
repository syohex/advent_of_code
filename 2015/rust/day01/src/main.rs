use std::fs;

fn what_floor(s: &str) -> i32 {
    s.chars()
        .fold(0, |acc, c| if c == '(' { acc + 1 } else { acc - 1 })
}

fn find_first_basement(s: &str) -> usize {
    let mut floor = 0;
    for (i, c) in s.chars().enumerate() {
        floor += if c == '(' { 1 } else { -1 };
        if floor == -1 {
            return i + 1;
        }
    }

    unreachable!();
}

fn main() {
    let input = fs::read_to_string("../../input/day01.txt").unwrap();
    let ret1 = what_floor(&input);
    let ret2 = find_first_basement(&input);

    println!("ret1={ret1}(expected: 232)");
    println!("ret2={ret2}(expected: 1783)");
}

#[test]
fn test1() {
    assert_eq!(what_floor("(())"), 0);
    assert_eq!(what_floor("()()"), 0);
    assert_eq!(what_floor("((("), 3);
    assert_eq!(what_floor("(()(()("), 3);
    assert_eq!(what_floor("())"), -1);
    assert_eq!(what_floor(")))"), -3);
    assert_eq!(what_floor(")())())"), -3);
}

#[test]
fn test2() {
    assert_eq!(find_first_basement(")"), 1);
    assert_eq!(find_first_basement("()())"), 5);
}
