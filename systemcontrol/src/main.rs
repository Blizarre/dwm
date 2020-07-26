use std::env;
use std::fs;

static BRIGHTNESS_PATH: &str = "/brightness";
static MAX_BRIGHTNESS_PATH: &str = "/max_brightness";

fn main() -> Result<(), ()> {
    let args: Vec<String> = env::args().collect();
    if args.len() != 4 {
        println!("Error: Missing arguments: brightness diff root_path.\nUsage:");
        println!("{} brightness +10 /sys/class/backlight/intel_backlight", args[0]);
        return Err(());
    }

    let module = args[1].as_str();
    let diff = args[2].as_str().parse::<i32>().expect("Invalid diff");
    let param = args[3].as_str();

    match module {
        "brightness" => {
            let brightness_max = fs::read_to_string(param.to_string() + MAX_BRIGHTNESS_PATH)
                .expect("Cannot read max brightness")
                .trim().parse::<i32>()
                .expect("Error parsing max brightness");

            let brightness = fs::read_to_string(param.to_string() + BRIGHTNESS_PATH)
                .expect("Cannot read current brightness")
                .trim().parse::<i32>()
                .expect("Error parsing current brightness");

            let new_brightness = brightness + diff;
            let new_brightness = std::cmp::min(
                std::cmp::max(new_brightness, 0),
                brightness_max
            );

            println!("Updating brightness {} -> {} (max {})", brightness, new_brightness, brightness_max);
            std::fs::write(param.to_string() + BRIGHTNESS_PATH, new_brightness.to_string())
                .expect("Error writing the new brightness");
        },
        _ => println!("Unknown module {}", module)
    }

    return Ok(());
}
