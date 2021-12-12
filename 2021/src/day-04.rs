use std::collections::HashSet;

#[derive(Debug, Clone, Hash, Eq, PartialEq)]
struct BingoSquare {
    is_marked: bool,
    value: u32,
}

#[test]
fn test_is_winner() {
    let column_winner = vec![
        BingoSquare {
            is_marked: true,
            value: 1,
        },
        BingoSquare {
            is_marked: false,
            value: 1,
        },
        BingoSquare {
            is_marked: true,
            value: 1,
        },
        BingoSquare {
            is_marked: false,
            value: 1,
        },
    ];
    let row_winner = vec![
        BingoSquare {
            is_marked: true,
            value: 1,
        },
        BingoSquare {
            is_marked: true,
            value: 1,
        },
        BingoSquare {
            is_marked: false,
            value: 1,
        },
        BingoSquare {
            is_marked: false,
            value: 1,
        },
    ];
    let not_a_winner = vec![
        BingoSquare {
            is_marked: false,
            value: 1,
        },
        BingoSquare {
            is_marked: true,
            value: 1,
        },
        BingoSquare {
            is_marked: true,
            value: 1,
        },
        BingoSquare {
            is_marked: false,
            value: 1,
        },
    ];
    assert_eq!(is_winner(&column_winner, 2), true);
    assert_eq!(is_winner(&row_winner, 2), true);
    assert_eq!(is_winner(&not_a_winner, 2), false);
}

fn is_winner(board: &[BingoSquare], size: usize) -> bool {
    let row_winner = board
        .chunks(size)
        .any(|row| row.iter().all(|square| square.is_marked));

    let column_winner = (0..size).any(|i| {
        board
            .iter()
            .skip(i)
            .step_by(size)
            .all(|square| square.is_marked)
    });
    row_winner || column_winner
}

fn part_one(
    numbers_to_draw: &[u32],
    mut boards: Vec<Vec<BingoSquare>>,
    size: usize,
) -> Option<u32> {
    for number in numbers_to_draw {
        for board in &mut boards {
            for square in &mut *board {
                if square.value == *number {
                    square.is_marked = true;
                }
            }
            if is_winner(board, size) {
                return Some(
                    board
                        .iter()
                        .filter(|b| !b.is_marked)
                        .map(|b| b.value)
                        .sum::<u32>()
                        * number,
                );
            }
        }
    }
    None
}

#[test]
fn test_part_two() {
    let numbers_to_draw = vec![
        7, 4, 9, 5, 11, 17, 23, 2, 0, 14, 21, 24, 10, 16, 13, 6, 15, 25, 12, 22, 18, 20, 8, 19, 3,
        26, 1,
    ];
    let boards: Vec<Vec<BingoSquare>> = vec![
        vec![
            22, 13, 17, 11, 0, 8, 2, 23, 4, 24, 21, 9, 14, 16, 7, 6, 10, 3, 18, 5, 1, 12, 20, 15,
            19,
        ]
        .iter()
        .map(|i| BingoSquare {
            is_marked: false,
            value: *i,
        })
        .collect(),
        vec![
            3, 15, 0, 2, 22, 9, 18, 13, 17, 5, 19, 8, 7, 25, 23, 20, 11, 10, 24, 4, 14, 21, 16, 12,
            6,
        ]
        .iter()
        .map(|i| BingoSquare {
            is_marked: false,
            value: *i,
        })
        .collect(),
        vec![
            14, 21, 17, 24, 4, 10, 16, 15, 9, 19, 18, 8, 23, 26, 20, 22, 11, 13, 6, 5, 2, 0, 12, 3,
            7,
        ]
        .iter()
        .map(|i| BingoSquare {
            is_marked: false,
            value: *i,
        })
        .collect(),
    ];
    assert_eq!(part_two(&numbers_to_draw, boards, 5).unwrap(), 1924)
}

fn part_two(
    numbers_to_draw: &[u32],
    mut boards: Vec<Vec<BingoSquare>>,
    size: usize,
) -> Option<u32> {
    let mut winners: HashSet<Vec<BingoSquare>> = HashSet::new();
    for number in numbers_to_draw {
        let number_of_boards = boards.len();
        for board in &mut boards {
            if winners.contains(board.as_slice()) {
                continue;
            }
            for square in &mut *board {
                if square.value == *number {
                    square.is_marked = true;
                }
            }
            if is_winner(board, size) {
                winners.insert(board.to_owned());
                if winners.len() == number_of_boards {
                    let sum = board
                        .iter()
                        .filter(|b| !b.is_marked)
                        .map(|b| b.value)
                        .sum::<u32>();
                    return Some(sum * number);
                }
            }
        }
    }
    None
}

fn main() {
    let input: Vec<&str> = include_str!("../input/day-04.txt").lines().collect();
    let (numbers_to_draw, boards) = input.split_first().unwrap();
    let size = 5;
    let boards = boards
        .chunks(size + 1)
        .fold(Vec::new(), |mut outer, chunk| {
            let board: Vec<BingoSquare> = chunk
                .join(" ")
                .split(' ')
                .filter(|c| !c.is_empty())
                .map(|f| {
                    let value = f.parse::<u32>().unwrap();
                    BingoSquare {
                        is_marked: false,
                        value,
                    }
                })
                .collect();
            outer.push(board);
            outer
        });

    let numbers_to_draw: Vec<u32> = numbers_to_draw
        .split(',')
        .map(|number| number.parse::<u32>().unwrap())
        .collect();

    println!("day four:");
    println!(
        "- part one: {}",
        part_one(&numbers_to_draw, boards.clone(), size).unwrap()
    );
    println!(
        "- part two: {}",
        part_two(&numbers_to_draw, boards, size).unwrap()
    );
}
