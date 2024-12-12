#include <iostream>
#include <vector>
#include <limits>

// Component structure
struct Component {
    std::string type;       // Component type (amplifier, switch, etc.)
    double gain;            // Gain or attenuation in dB
    double cost;            // Cost in USD
    double p1dB;            // Power handling capability (for amplifiers)
    double leakage;         // Leakage for switches (OFF state)
};

// Function to evaluate if a configuration meets requirements
bool isValidConfiguration(const std::vector<Component>& configuration, double requiredGain, double maxLeakage, double maxPower) {
    double totalGain = 0.0;
    double totalLeakage = 0.0;

    for (const auto& comp : configuration) {
        if (comp.type == "amplifier" || comp.type == "attenuator") {
            totalGain += comp.gain;
        }
        if (comp.type == "switch") {
            totalLeakage += comp.leakage;
        }
        if (comp.type == "amplifier" && totalGain > comp.p1dB) {
            return false;  // Exceeds power handling capability
        }
    }

    // Ensure gain, leakage, and dual output constraints are met
    return (totalGain >= requiredGain && totalLeakage <= maxLeakage);
}

int main() {
    // Available components (example values)
    std::vector<Component> components = {
        {"amplifier", 20.0, 50.0, 10.0, 0.0},
        {"switch", -1.0, 10.0, 0.0, 0.01},
        {"variable attenuator", -10.0, 20.0, 0.0, 0.0},
        {"fixed attenuator", -5.0, 5.0, 0.0, 0.0},
        {"power divider", -3.0, 15.0, 0.0, 0.0}  // Required for dual output
    };

    // Specifications
    double requiredGain = 15.0;      // Required gain (in dB)
    double maxLeakage = 0.05;        // Maximum acceptable leakage
    double maxPower = 10.0;          // Maximum power handling capability

    std::vector<Component> bestConfiguration;
    double minCost = std::numeric_limits<double>::max();

    // Iterate over all possible configurations
    for (size_t i = 0; i < components.size(); ++i) {
        for (size_t j = 0; j < components.size(); ++j) {
            for (size_t k = 0; k < components.size(); ++k) {
                // Ensure the power divider is included in the configuration
                if (components[i].type != "power divider" && components[j].type != "power divider" && components[k].type != "power divider") {
                    continue;
                }

                std::vector<Component> configuration = {components[i], components[j], components[k]};

                // Check if the configuration meets the requirements
                if (isValidConfiguration(configuration, requiredGain, maxLeakage, maxPower)) {
                    // Calculate the total cost of this configuration
                    double totalCost = 0.0;
                    for (const auto& comp : configuration) {
                        totalCost += comp.cost;
                    }

                    // Update the best configuration if this one is cheaper
                    if (totalCost < minCost) {
                        minCost = totalCost;
                        bestConfiguration = configuration;
                    }
                }
            }
        }
    }

    // Output the best configuration
    std::cout << "Best Configuration:\n";
    for (const auto& comp : bestConfiguration) {
        std::cout << "Type: " << comp.type << ", Gain: " << comp.gain << " dB, Cost: $" << comp.cost << "\n";
    }
    std::cout << "Total Cost: $" << minCost << "\n";

    return 0;
}
