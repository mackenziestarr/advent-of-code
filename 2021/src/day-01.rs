fn part_one(input: &Vec<i32>) -> usize {
    input
        .windows(2)
        .map(|w| w[1] - w[0]) // adjacent difference
        .filter(|i| *i > 0)
        .count()
}

fn part_two(input: &Vec<i32>) -> usize {
    input
        .windows(3)
        .map(|w| w[2] + w[1] + w[0])
        .collect::<Vec<i32>>()
        .windows(2)
        .map(|w| w[1] - w[0]) // adjacent difference
        .filter(|i| *i > 0)
        .count()
}

fn main() {
    let input: Vec<i32> = include_str!("../input/day-01.txt")
        .lines()
        .map(|s| s.parse::<i32>().unwrap())
        .collect();

    println!("day one:");
    println!("- part one: {}", part_one(&input));
    println!("- part two: {}", part_two(&input));
}
