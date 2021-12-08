fn count_bits_at_position(input: &[u32], position: usize) -> (u32, u32) {
    input.iter().fold((0, 0), |result, i| {
        let (zeros, ones) = result;
        if i & (1 << position) != 0 {
            (zeros, ones + 1)
        } else {
            (zeros + 1, ones)
        }
    })
}

fn part_one(input: &[u32], width: usize) -> (u32, u32) {
    let mut result = 0;
    for position in 0..width {
        let (zeros, ones) = count_bits_at_position(input, position);
        if ones > zeros {
            result |= 1 << position
        } else {
            result &= !(1 << position)
        }
    }
    let gamma_rate = result;
    let epsilon_rate = !(u32::MAX << width) & !result;
    (gamma_rate, epsilon_rate)
}

fn part_two(input: &[u32], width: usize) -> (u32, u32) {
    let oxygen_generator_rating = (0..width).rfold(input.to_owned(), |input, position| {
        if input.len() == 1 {
            input
        } else {
            let (zeros, ones) = count_bits_at_position(&input, position);
            input
                .into_iter()
                .filter(|i| {
                    let bit_is_set = *i & (1 << position) != 0;
                    if zeros > ones {
                        !bit_is_set
                    } else {
                        bit_is_set
                    }
                })
                .collect()
        }
    });
    let co2_scrubber_rating = (0..width).rfold(input.to_owned(), |input, position| {
        if input.len() == 1 {
            input
        } else {
            let (zeros, ones) = count_bits_at_position(&input, position);
            input
                .into_iter()
                .filter(|i| {
                    if zeros <= ones {
                        *i & (1 << position) == 0
                    } else {
                        *i & (1 << position) != 0
                    }
                })
                .collect()
        }
    });
    (oxygen_generator_rating[0], co2_scrubber_rating[0])
}

#[test]
fn test() {
    let width: usize = 5;
    let input = vec![
        0b00100, 0b11110, 0b10110, 0b10111, 0b10101, 0b01111, 0b00111, 0b11100, 0b10000, 0b11001,
        0b00010, 0b01010,
    ];

    let (gamma_rate, epsilon_rate) = part_one(&input, width);
    assert_eq!(gamma_rate * epsilon_rate, 198);

    let (oxygen_generator_rate, co2_scrubber_rating) = part_two(&input, width);
    assert_eq!(oxygen_generator_rate * co2_scrubber_rating, 230);
}

fn main() {
    let input: Vec<&str> = include_str!("../input/day-03.txt").lines().collect();
    let width: usize = input[0].len();

    let numeric_input: Vec<u32> = input
        .iter()
        .map(|x| u32::from_str_radix(*x, 2).unwrap())
        .collect();

    println!("day three:");

    let (gamma_rate, epsilon_rate) = part_one(&numeric_input, width);
    println!("- part one: {}", gamma_rate * epsilon_rate);

    let (oxygen_generator_rate, co2_scrubber_rating) = part_two(&numeric_input, width);
    println!(
        "- part two: {}",
        oxygen_generator_rate * co2_scrubber_rating
    );
}
