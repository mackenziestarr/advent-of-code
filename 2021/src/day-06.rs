#[test]
fn test_part_one() {
    let input: Vec<u8> = vec![3, 4, 3, 1, 2];
    assert_eq!(part_one(input.clone(), 80), 5934);
    assert_eq!(part_two(input, 256), 26_984_457_539u64);
}

fn part_one(mut input: Vec<u8>, days: usize) -> usize {
    for _ in 0..days {
        let size = input.len();
        let mut new_fish: Vec<u8> = Vec::new();
        for i in &mut input {
            if *i == 0u8 {
                new_fish.push(8u8);
                *i = 6;
            } else {
                *i -= 1;
            }
        }
        input.append(&mut new_fish);
    }
    input.iter().count()
}

fn part_two(mut input: Vec<u8>, days: usize) -> u64 {
    let mut counts: [u64; 9] = [0; 9];
    for i in input {
        counts[i as usize] += 1;
    }
    for _ in 0..days {
        let new_fish = counts[0];
        for i in 1..9 {
            counts[i-1] = counts[i];
        }
        counts[8] = new_fish;
        counts[6] += new_fish;
    }
    counts.iter().sum()
}

fn main() {
    let input: Vec<u8> = include_str!("../input/day-06.txt")
        .trim()
        .split(',')
        .map(|n| n.parse::<u8>().unwrap_or(0))
        .collect();
    println!("day six:");
    println!("- part one: {}", part_one(input.clone(), 80));
    println!("- part two: {}", part_two(input.clone(), 256));
}