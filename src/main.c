#include "app.h"
#include "nbody.h"
#include "camera.h"
#include "render.h"


int main() {
    Application app = app_init("N-Body Simulation", 960, 540);
    
    do {
        app.restart = 0;

        // Initialize
        Camera cam = camera_init();
        Bodies bod = bodies_init();
        Render mesh = render_init(bod.points, bod.colors);
        Compute comp = compute_init(mesh.vboPositions, bod.velocity, bod.mass);

        shader_pipeline(&app);

        // Main Loop
        while (!app_quit(&app) && !app.restart) {
            app_input(&app, &cam);
            camera_update(&cam);
            render_frame(&app, &cam, &mesh, &bod);

            if (app.toggle == 1) {
                update_positions(&comp);
            }
        }

        // Destroy
        compute_destroy(&comp);
        bodies_destroy(&bod);
    } while(app.restart);

    app_destroy(&app);
    return 0;
}