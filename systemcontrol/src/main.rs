use std::env;
use std::fs;

static BRIGHTNESS_PATH: &str = "/sys/class/backlight/intel_backlight/brightness";
static MAX_BRIGHTNESS_PATH: &str = "/sys/class/backlight/intel_backlight/max_brightness";

fn main() -> Result<(), ()> {
    let args: Vec<String> = env::args().collect();
    if args.len() != 3 {
        println!("Error: Missing arguments: brightness/volume up/down.\nUsage:");
        println!("{} brightness up", args[0]);
        return Err(());
    }

    let module = args[1].as_str();
    let action = args[2].as_str();

    match module {
        "brightness" => {
            let brightness_max = fs::read_to_string(MAX_BRIGHTNESS_PATH)
                .expect("Cannot read max brightness")
                .trim().parse::<i32>()
                .expect("Error parsing max brightness");

            let brightness = fs::read_to_string(BRIGHTNESS_PATH)
                .expect("Cannot read current brightness")
                .trim().parse::<i32>()
                .expect("Error parsing current brightness");

            let new_brightness = match action {
                "up" => std::cmp::min(brightness_max, brightness + 100),
                "down" => std::cmp::max(0, brightness - 100),
                _ => panic!(format!("Error: Unknown action {}", action))
            };
            println!("Updating brightness {} -> {} (max {})", brightness, new_brightness, brightness_max);
            std::fs::write(BRIGHTNESS_PATH, new_brightness.to_string())
                .expect("Error writing the new brightness");
        },
        _ => println!("Unknown module {}", module)
    }

    return Ok(());
}
