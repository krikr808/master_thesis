////////////////////////////////////////////////////////////////
// INCLUDE FILES
////////////////////////////////////////////////////////////////

// Standard includes
#include <iostream>
using std::cout;
using std::endl;

// Own include files
#include "fvoctree.h"

////////////////////////////////////////////////////////////////
// CONSTRUCTORS AND DESTRUCTOR
////////////////////////////////////////////////////////////////

fvoctree::fvoctree()
{
    root = 0;
}

fvoctree::fvoctree(pftype surface, pftype bottom)
{
    bottom = bottom;
    surface = surface;
    octcell *c = root = new octcell(0, 1, pfvec(), 0);
    refine_subtree(c, surface, bottom);
    prepare_cells_for_water_recursively(c);
}

fvoctree::~fvoctree()
{
    if (root) {
        delete root;
    }
}

////////////////////////////////////////////////////////////////
// PRIVATE NON-STATIC METHODS
////////////////////////////////////////////////////////////////

/* Returns true if the cell should be removed from the simulation */
bool fvoctree::refine_subtree(octcell* c, pftype surface, pftype bottom)
{
    static int tot_num_cells = 1;
    static int num_leaf_cells = 1;
    pftype s = c->s;

    pftype lowest_cell_height = c->r[VERTICAL_DIMENSION];
    pftype min_max_balance = 0.5;
#if    NUM_DIMENSIONS == 2
#if 1 // Step profile surface
    //pftype hdiff = .24118956;
    pftype hdiff = 14.0/64;
    pftype step_x_pos = 45.0/64;
    pftype low = SURFACE_HEIGHT;
    pftype high = SURFACE_HEIGHT + hdiff;
    pftype min_surf_height;
    pftype max_surf_height;

    if (c->r[HORIZONTAL_DIMENSION1] < step_x_pos) {
        min_surf_height = low;
        if (c->get_opposite_corner()[HORIZONTAL_DIMENSION1] < step_x_pos) {
            max_surf_height = low;
        }
        else {
            max_surf_height = high;
            min_max_balance = (step_x_pos - c->r[HORIZONTAL_DIMENSION1])/c->get_edge_length();
        }
    }
    else {
        min_surf_height = max_surf_height = high;
    }

    /*
    pftype local_surface_height = c->get_cell_center().e[HORIZONTAL_DIMENSION1] > step_x_pos ? SURFACE_HEIGHT + hdiff/2 : SURFACE_HEIGHT - hdiff/2;
    pftype min_surf_height = local_surface_height;
    pftype max_surf_height = local_surface_height;
    */
#elif 0 // Pilar
#if 0 // Large pilar
    pftype hdiff = .5;
    pftype width = .5;
#elif 0 // High, thin pilar
    pftype hdiff = .5;
    pftype width = .1;
#else  // Small pilar
    pftype hdiff = .1;
    pftype width = .2;
#endif
    pftype low  = SURFACE_HEIGHT - hdiff/2;
    pftype high = SURFACE_HEIGHT + hdiff/2;
    pftype min_surf_height, max_surf_height;
    if (c->r.e[HORIZONTAL_DIMENSION1] < .5 - width/2) {
        min_surf_height = low;
        if (c->get_opposite_corner().e[HORIZONTAL_DIMENSION1] > .5 - width/2) {
            max_surf_height = high;
        }
        else {
            max_surf_height = low;
        }
    }
    else if (c->get_opposite_corner().e[HORIZONTAL_DIMENSION1] > .5 + width/2) {
        min_surf_height = low;
        if (c->r.e[HORIZONTAL_DIMENSION1] < .5 + width/2) {
            max_surf_height = high;
        }
        else {
            max_surf_height = low;
        }
    }
    else {
        min_surf_height = max_surf_height = high;
    }

#else // Sloping surface
#if 1 // Slope to the right
    pftype left_edge_height = 0.5501;
    pftype slope = 0.4;
#else // Slope to the left
    pftype left_edge_height = 0.9501;
    pftype slope = -0.4;
#endif
    pftype min_surf_height = left_edge_height + slope*(c->r[HORIZONTAL_DIMENSION1] + (slope < 0 ? s : pftype(0)));
    pftype max_surf_height = left_edge_height + slope*(c->r[HORIZONTAL_DIMENSION1] + (slope > 0 ? s : pftype(0)));
#endif
#elif  NUM_DIMENSIONS == 3
    pftype min_surf_height = 0.55 + 0.2*c->r[HORIZONTAL_DIMENSION1] + 0.1*c->r[HORIZONTAL_DIMENSION2];
    pftype max_surf_height = 0.55 + 0.2*(c->r[HORIZONTAL_DIMENSION1]+s) + 0.1*(c->r[HORIZONTAL_DIMENSION2]+s);
#endif
    if (lowest_cell_height >= max_surf_height) {
        // Cell is over the surface, remove it
        return true;
    }
#if 0
    if (lowest_cell_height + s <= min_surf_height     ||  // Cell is under the surface, keep it but stop refining
        s <= accuracy_function(c->cell_center()) ) { // Accuracy is good enough, stop refining
#else
    if (c->is_fine_enough()) { // Accuracy is good enough, stop refining
#endif
        /* Stop refining */
        /* Calculate properties */
        pftype beta; /* Water volume divided by the volume of the cell */
        pftype mean_height = lowest_cell_height + s/2;
        pftype mean_surface_height = (min_surf_height + max_surf_height)/2;
        pftype depth = mean_surface_height - mean_height;
        if (depth >= 0) {
            c->p = NORMAL_AIR_PRESSURE + depth * (P_G * NORMAL_WATER_DENSITY);
        }
        else {
            c->p = NORMAL_AIR_PRESSURE * exp(depth * (P_G / AIR_COMPRESSIBILITY_FACTOR));
        }
        if (lowest_cell_height + s > min_surf_height) {
            /* Cell is a surface cell */
            /* Estimate beta */
            pftype mean_height_diff = min_max_balance*MAX(min_surf_height - lowest_cell_height, 0) + (1-min_max_balance)*MIN(pftype(max_surf_height - lowest_cell_height), s);
            beta = mean_height_diff / c->s;
        }
        else {
            /* Cell is not a surface cell and contains only water */
            beta = 1;
        }
        pftype water_vol_coeff = 1 + (c->p - NORMAL_AIR_PRESSURE)*(1/ARTIFICIAL_COMPRESSIBILITY_FACTOR);
#if  NO_ATMOSPHERE
        pftype air_vol_coeff = water_vol_coeff;
#else
        pftype air_vol_coeff = c->p / NORMAL_AIR_PRESSURE;
#endif
        water_vol_coeff *= beta;
        air_vol_coeff   *= (1 - beta);
        c->set_volume_coefficients(water_vol_coeff, water_vol_coeff + air_vol_coeff);
        return false;
    }
    // Cell is not fine enough, refine it and then handle the children recursivelly
    c->refine();
    tot_num_cells += octcell::MAX_NUM_CHILDREN;
    num_leaf_cells += octcell::MAX_NUM_CHILDREN - 1;
    for (uint i = 0; i < octcell::MAX_NUM_CHILDREN; i++) {
        if (refine_subtree(c->get_child(i), surface, bottom)) {
            c->remove_child(i);
            tot_num_cells--;
            num_leaf_cells--;
        }
    }
#if  DEBUG
#if  0
    cout << "Total number of cells: " << tot_num_cells << endl;
    cout << "Number of leaf cells:  " << num_leaf_cells << endl;
    cout << endl;
#endif
    if (!c->get_number_of_children()) {
        throw logic_error("Cell has no children and is not a leaf cell, yet it is allowed to exist");
    }
#endif
    return 0;
}

void fvoctree::prepare_cells_for_water_recursively(octcell* cell)
{
    if (cell->is_leaf()) {
        if (cell->has_water()) {
            cell->prepare_for_water();
        }
        return;
    }
    for (uint i = 0; i < octcell::MAX_NUM_CHILDREN; i++) {
        if (cell->has_child(i)) {
            prepare_cells_for_water_recursively(cell->get_child(i));
        }
    }
}
