//////////////////////////////////////////////////////////////////////////////////////
//       _______.  ______  __       ___      .__   __. .___________. __  ___   ___  //
//      /       | /      ||  |     /   \     |  \ |  | |           ||  | \  \ /  /  //
//     |   (----`|  ,----'|  |    /  ^  \    |   \|  | `---|  |----`|  |  \  V  /   //
//      \   \    |  |     |  |   /  /_\  \   |  . `  |     |  |     |  |   >   <    //
//  .----)   |   |  `----.|  |  /  _____  \  |  |\   |     |  |     |  |  /  .  \   //
//  |_______/     \______||__| /__/     \__\ |__| \__|     |__|     |__| /__/ \__\  //
//                                                                                  //
//  Originally developed by D. Pizzocri & T. Barani                                 //
//                                                                                  //
//  Version: 2.1                                                                    //
//  Year: 2024                                                                      //
//  Authors: D. Pizzocri, G. Zullo.                                                 //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////

#include "Simulation.h"

void Simulation::HighBurnupStructureFormation()
{
    if (!int(input_variable["iHighBurnupStructureFormation"].getValue())) return;

    // Model declaration
    Model model_;

    model_.setName("High-burnup structure formation");

    std::string reference;
    std::vector<double> parameter;

    switch (int(input_variable["iHighBurnupStructureFormation"].getValue()))
    {
        case 0:
        {
            reference += ": not considered.";
            parameter.push_back(0.0);
            parameter.push_back(0.0);
            parameter.push_back(0.0);
            parameter.push_back(0.0);

            break;
        }

        case 1:
        {
            reference += ": Barani et al. Journal of Nuclear Materials 539 (2020) 152296";

            double avrami_constant(3.54);
            double transformation_rate(2.77e-7);
            double resolution_layer_thickness = 1.0e-9; //(m)
            double resolution_critical_distance = 1.0e-9; //(m)

            parameter.push_back(avrami_constant);
            parameter.push_back(transformation_rate);
            parameter.push_back(resolution_layer_thickness);
            parameter.push_back(resolution_critical_distance);

            break;
        }

        default:
            ErrorMessages::Switch(__FILE__, "iHighBurnupStructureFormation", int(input_variable["iHighBurnupStructureFormation"].getValue()));
            break;
    }

    model_.setParameter(parameter);
    model_.setRef(reference);

    model.push(model_);

    // Model resolution
    // Restructuring rate:
    // dalpha_r / bu = 3.54 * 2.77e-7 (1-alpha_r) b^2.54
    double coefficient =
        model["High-burnup structure formation"].getParameter().at(0) *
        model["High-burnup structure formation"].getParameter().at(1) *
        pow(sciantix_variable["Effective burnup"].getFinalValue(), 2.54);

    sciantix_variable["Restructured volume fraction"].setFinalValue(
        solver.Decay(
            sciantix_variable["Restructured volume fraction"].getInitialValue(),
            coefficient,
            coefficient,
            sciantix_variable["Effective burnup"].getIncrement()
        )
    );
}