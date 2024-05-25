#define MAX_SEGMENTS 12 // Define the maximum number of segments

// Define an array to hold the endpoints of the lines
float lineEndpointsAwayFromCircle[MAX_SEGMENTS][2]; // Each row represents an endpoint [x, y]
float lineEndpointsNearToCircle[MAX_SEGMENTS][2];   // Each row represents an endpoint [x, y]

int endpointIndex = 0; // Track the index of the current endpoint

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
const float outerMostCircleRadius = WINDOW_WIDTH / 2 - 200;
const float innerCircleRadius = outerMostCircleRadius - 50;
const int border_width = 10;
const int centerX = WINDOW_WIDTH / 2;
const int centerY = WINDOW_HEIGHT / 2;

float skyBlue[] = {169.0f / 255.0f, 198.0f / 255.0f, 228.0f / 255.0f, 1.0f};   // RGB(169, 198, 228) with full opacity
float blueish[] = {2.0f / 255.0f, 84.0f / 255.0f, 166.0f / 255.0f, 1.0f};      // RGB(2,84,166) with full opacity
float yellow[] = {244.0f / 255.0f, 232.0f / 255.0f, 112.0f / 255.0f, 1.0f};    // RGB(244, 232, 112) with full opPacity
float black[] = {0.0f, 0.0f, 0.0f, 1.0f};                                      // RGB(0, 0, 0) with full opacity
float white[] = {1.0f, 1.0f, 1.0f, 1.0f};                                      // RGB(1, 1, 1) with full opacity
float red[] = {1.0f, 0.0f, 0.0f, 1.0f};                                        // RGB(1, 0, 0) with full opacity
float lightPink[] = {253.0f / 255.0f, 173.0f / 255.0f, 172.0f / 255.0f, 1.0f}; // RGB(253, 173, 172) with full opacity
float transparent[] = {1.0f, 1.0f, 1.0f, 0.0f};                                // Transparent white
