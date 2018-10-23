#include "Memory.h"
#include "WitnessRandomizer.h"
#include <string>

int main(int argc, char** argv)
{
	WitnessRandomizer randomizer = WitnessRandomizer();

	//*
	if (argc == 2) {
		srand(atoi(argv[1])); // Seed with RNG from command line
	}

	std::vector<int> tutorialPanels = {
		// 0x0A3B5, // Back Left
		0x0A3B2, // Back Right
		0x00295, // Center Left
		0x00293, // Front Center
		0x002C2, // Front Left
		0x0C335, // Pillar
		0x0C373, // Patio floor
	};

	randomizer.Randomize(tutorialPanels, SWAP_TARGETS);


	std::vector<int> panels = {
		0x3C12B, // Glass Factory Discard
		0x01A54, // Glass Factory Entry
		0x00086, // Glass Factory Vertical Symmetry 1
		0x00087, // Glass Factory Vertical Symmetry 2
		0x00059, // Glass Factory Vertical Symmetry 3
		0x00062, // Glass Factory Vertical Symmetry 4
		0x0005C, // Glass Factory Vertical Symmetry 5
		// 0x17CC8, // Glass Factory Summon Boat
		0x0008D, // Glass Factory Rotational Symmetry 1
		0x00081, // Glass Factory Rotational Symmetry 2
		0x00083, // Glass Factory Rotational Symmetry 3
		0x00084, // Glass Factory Melting 1
		0x00082, // Glass Factory Melting 2
		0x0343A, // Glass Factory Melting 3

		0x000B0, // Symmetry Island Door 1
		0x00022, // Symmetry Island Black Dots 1
		0x00023, // Symmetry Island Black Dots 2
		0x00024, // Symmetry Island Black Dots 3
		0x00025, // Symmetry Island Black Dots 4
		0x00026, // Symmetry Island Black Dots 5
		0x0007C, // Symmetry Island Colored Dots 1
		0x0007E, // Symmetry Island Colored Dots 2
		0x00075, // Symmetry Island Colored Dots 3
		0x00073, // Symmetry Island Colored Dots 4
		0x00077, // Symmetry Island Colored Dots 5
		0x00079, // Symmetry Island Colored Dots 6
		0x00065, // Symmetry Island Fading Lines 1
		0x0006D, // Symmetry Island Fading Lines 2
		0x00072, // Symmetry Island Fading Lines 3
		0x0006F, // Symmetry Island Fading Lines 4
		0x00070, // Symmetry Island Fading Lines 5
		0x00071, // Symmetry Island Fading Lines 6
		0x00076, // Symmetry Island Fading Lines 7
		// 0x009B8, // Symmetry Island Transparent 1 // Too mean for right now
		// 0x003E8, // Symmetry Island Transparent 2 // Too mean for right now
		// 0x00A15, // Symmetry Island Transparent 3 // Too mean for right now
		// 0x00B53, // Symmetry Island Transparent 4 // Too mean for right now
		// 0x00B8D, // Symmetry Island Transparent 5 // Too mean for right now
		// 0x1C349, // Symmetry Island Door 2 - Collision fails here, sadly
		0x00A52, // Symmetry Island Laser Yellow 1
		0x00A57, // Symmetry Island Laser Yellow 2
		0x00A5B, // Symmetry Island Laser Yellow 3
		0x00A61, // Symmetry Island Laser Blue 1
		0x00A64, // Symmetry Island Laser Blue 2
		0x00A68, // Symmetry Island Laser Blue 3
		// 0x0360D, // Symmetry Island Laser
	};
	randomizer.Randomize(panels, SWAP_PATHWAYS | SWAP_COLORS);
	/*/


	int GF_DOOR = 0x1A54;
	int SI_2_1 = 0x7C;
	int SI_2_2 = 0x7E;
	int SI_2_3 = 0x75;
	int GF_DISC = 0x3C12B;

	randomizer.SwapPanels(GF_DOOR, SI_2_3, SWAP_PATHWAYS | SWAP_DECORATIONS | SWAP_COLORS);
	//*/
}

void WitnessRandomizer::Randomize(std::vector<int> panels, int flags) {
	for (size_t i=panels.size() - 1; i > 1; i--) {
		int target = rand() % i;
		if (i != target) {
			SwapPanels(panels[i], panels[target], flags);
			std::swap(panels[i], panels[target]); // Panel indices in the array
		}
	}
}

void WitnessRandomizer::SwapPanels(int panel1, int panel2, int flags) {
	std::map<int, int> offsets;

	if (flags & SWAP_TARGETS) {
		offsets[0x2BC] = sizeof(int);
	}
	if (flags & SWAP_PATHWAYS) {
		offsets[0x3B8] = sizeof(int); // num_dots
		offsets[0x3BC] = sizeof(int); // num_connections
		offsets[0x3C8] = sizeof(void*); // *dot_positions
		offsets[0x3D0] = sizeof(void*); // *dot_flags
		offsets[0x3D8] = sizeof(void*); // *dot_connection_a
		offsets[0x3E0] = sizeof(void*); // *dot_connection_b
		offsets[0x440] = sizeof(void*); // *reflection_data
		offsets[0x448] = sizeof(int); // grid_size_x
		offsets[0x44C] = sizeof(int); // grid_size_y
		offsets[0x450] = sizeof(int); // style_flags // This is required to not ignore dots
		offsets[0x4B0] = sizeof(void*); // *panel_target

		offsets[0x420] = sizeof(void*); // *decorations
		offsets[0x428] = sizeof(void*); // *decoration_flags
		offsets[0x438] = sizeof(int); // num_decorations
									  
		// TODO: Try using is_cylinder to swap into tutorial pillar. If it fails, discard.
		// Probably not: Extra back distance
		// SwapPanelData(panels[i], panels[target], 0x2FC, 125); // is_cylinder through max_connections
	}
	if (flags & SWAP_STYLE) {
		// 340 - pattern_scale
		// 3A4 - path_width_scale
		// 3A8 - startpoint_scale
		// 340 - pattern_scale
		// 288 - flash_mode
	}
	if (flags & SWAP_COLORS) {
		offsets[0xC8] = 16; // path_color
		offsets[0xD8] = 16; // reflection_path_color
//		offsets[0xE8] = 16; // deprecated_finished_path_color
		offsets[0xF8] = 16; // dot_color
		offsets[0x108] = 16; // active_color
		offsets[0x118] = 16; // background_region_color
		offsets[0x128] = 16; // success_color_a
		offsets[0x138] = 16; // success_color_b
		offsets[0x148] = 16; // strobe_color_a
		offsets[0x158] = 16; // strobe_color_b
		offsets[0x168] = 16; // error_color
//		offsets[0x178] = 16; // video_status_color
		offsets[0x188] = 16; // pattern_point_color
		offsets[0x198] = 16; // pattern_point_color_a
		offsets[0x1A8] = 16; // pattern_point_color_b
//		offsets[0x1B8] = 16; // symbol_a
//		offsets[0x1C8] = 16; // symbol_b
//		offsets[0x1D8] = 16; // symbol_c
//		offsets[0x1E8] = 16; // symbol_d
//		offsets[0x1F8] = 16; // symbol_e
//		offsets[0x208] = sizeof(float); // push_symbol_colors
//		offsets[0x20C] = 16; // outer_background
//		offsets[0x108] = sizeof(int); // outer_background_mode
	}
	if (flags & SWAP_TRACED) {
		offsets[0x230] = 16; // traced_edges
	}


	for (auto const& [offset, size] : offsets) {
		SwapPanelData(panel1, panel2, offset, size);
	}
}

void WitnessRandomizer::SwapPanelData(int panel1, int panel2, int finalOffset, int dataSize) {
	// Currently wired for old version
	std::vector<int> panel1Offset = {0x5B28C0, 0x18, panel1*8, finalOffset};
	std::vector<int> panel2Offset = {0x5B28C0, 0x18, panel2*8, finalOffset};

	std::vector<byte> panel1Data = _memory.ReadData<byte>(panel1Offset, dataSize);
	std::vector<byte> panel2Data = _memory.ReadData<byte>(panel2Offset, dataSize);
	_memory.WriteData<byte>(panel2Offset, panel1Data);
	_memory.WriteData<byte>(panel1Offset, panel2Data);
}
