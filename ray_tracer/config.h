#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>
#include <algorithm>
#include "types.h"

struct Config
{
    // "General" section
    int startDistance = 100;

    // "Window" section
    WindowConfig window;

    // "Materials" section
    // We store them by name in a dictionary/map
    std::unordered_map<std::string, Material> materials;

    // "Spheres" section
    std::vector<Sphere> spheres;

    // "Lights" section
    std::vector<Light> lights;
};

Config parseConfig(const std::string &filename)
{
    Config config; // This will hold all parsed values

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Cannot open config file: " << filename << std::endl;
        return config; // return defaults
    }

    enum class Section
    {
        None,
        General,
        Window,
        Materials,
        Spheres,
        Lights
    };

    Section currentSection = Section::None;
    std::string line;

    while (std::getline(file, line))
    {
        // Trim whitespace
        // (A simple approach if you want a quick trim; or do a more robust approach)
        if (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
        {
            line.pop_back();
        }

        // Skip empty lines or comment lines
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        // Check if this line is a section header, e.g. "General:"
        // Convert line to lowercase for simple matching if you want
        if (line == "General:")
        {
            currentSection = Section::General;
            continue;
        }
        else if (line == "Window:")
        {
            currentSection = Section::Window;
            continue;
        }
        else if (line == "Materials:")
        {
            currentSection = Section::Materials;
            continue;
        }
        else if (line == "Spheres:")
        {
            currentSection = Section::Spheres;
            continue;
        }
        else if (line == "Lights:")
        {
            currentSection = Section::Lights;
            continue;
        }

        // Otherwise, parse the line according to the current section
        switch (currentSection)
        {
        case Section::General:
        {
            // Example format: "start_distance = 100"
            // We can parse key = value
            std::istringstream iss(line);
            std::string key;
            char eq;
            iss >> key >> eq; // eq should be '='
            if (key == "start_distance")
            {
                int val;
                iss >> val;
                config.startDistance = val;
            }
            break;
        }

        case Section::Window:
        {
            // Example format:
            // x = 0
            // y = 20
            // width = 700
            // height = 700
            std::istringstream iss(line);
            std::string key;
            char eq;
            iss >> key >> eq;
            if (key == "x")
            {
                iss >> config.window.x;
            }
            else if (key == "y")
            {
                iss >> config.window.y;
            }
            else if (key == "width")
            {
                iss >> config.window.width;
            }
            else if (key == "height")
            {
                iss >> config.window.height;
            }
            break;
        }

        case Section::Materials:
        {
            // Example format:
            // red = 1.0 0.0 0.0 0.5
            //    name    r    g    b  reflexivity
            // We can parse "name = r g b refl"
            // Or if you want a custom syntax, adapt accordingly
            std::istringstream iss(line);
            std::string materialName;
            char eq;
            float r, g, b, refl;
            iss >> materialName >> eq >> r >> g >> b >> refl;

            Material mat;
            mat.color = HDColor(r, g, b, 1.0f);
            mat.reflexivity = refl;

            config.materials[materialName] = mat;
            break;
        }

        case Section::Spheres:
        {
            // Example format:
            // -1000 0 1300 red 1000
            // center_x center_y center_z materialName radius
            std::istringstream iss(line);
            float cx, cy, cz, radius;
            std::string matName;
            iss >> cx >> cy >> cz >> matName >> radius;

            // Look up the material
            Material sphereMat;
            if (auto it = config.materials.find(matName); it != config.materials.end())
            {
                sphereMat = it->second;
            }
            else
            {
                // Not found; in real code, handle error or use a default
                std::cerr << "[WARNING] Material " << matName << " not found. Using default.\n";
            }

            config.spheres.emplace_back(
                Vector3(cx, cy, cz),
                sphereMat,
                radius);
            break;
        }

        case Section::Lights:
        {
            // Example format:
            // -300 -300 100 1.0
            // pos_x pos_y pos_z intensity
            std::istringstream iss(line);
            float lx, ly, lz, intensity;
            iss >> lx >> ly >> lz >> intensity;

            config.lights.emplace_back(Vector3(lx, ly, lz), intensity);
            break;
        }

        default:
            // Section::None or unrecognized
            break;
        }
    }

    file.close();
    return config;
}
