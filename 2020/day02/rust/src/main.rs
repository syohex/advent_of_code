use std::io::{self, Read};

#[derive(Debug)]
struct Line<'a> {
    min: usize,
    max: usize,
    chr: &'a str,
    message: &'a str,
}

impl<'a> Line<'a> {
    fn valid(&self) -> bool {
        let count = self.message.matches(self.chr).count();
        count >= self.min && count <= self.max
    }
}

fn solve01(lines: &Vec<Line>) -> usize {
    lines.iter().filter(|&line| line.valid()).count()
}

fn parse_input(input: &str) -> Vec<Line> {
    let mut ret: Vec<Line> = Vec::new();
    for line in input.lines() {
        let parts: Vec<&str> = line.split_whitespace().collect();
        assert!(parts.len() == 3);

        let counts: Vec<usize> = parts[0].split('-').flat_map(|s| s.parse().ok()).collect();
        ret.push(Line {
            min: counts[0],
            max: counts[1],
            chr: &parts[1][0..1],
            message: parts[2],
        });
    }

    ret
}

fn main() -> io::Result<()> {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input)?;

    let lines = parse_input(&input);
    let answer1 = solve01(&lines);
    assert!(answer1 == 580);

    println!("Part01: {}", answer1);
    Ok(())
}

#[test]
fn test01() {
    let input = r#"1-3 a: abcde
1-3 b: cdefg
2-9 c: ccccccccc"#;

    let lines = parse_input(input);
    assert_eq!(solve01(&lines), 2);
}
