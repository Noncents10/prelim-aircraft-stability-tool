#include <stdio.h>
#include <math.h>
#include "plot.h"
#include "stability.h"

static double dmax(double a, double b) { return a > b ? a : b; }
static double dmin(double a, double b) { return a < b ? a : b; }

int write_aircraft_svg(const char *path, const Aircraft *a)
{
    const double xnp = neutral_point_x(a);
    const double x_tail_ac = a->x_cg + a->l_t;

    /* Determine drawing bounds (in metres) */
    double xmin = dmin(dmin(a->x_ac_w, a->x_cg), dmin(xnp, x_tail_ac));
    double xmax = dmax(dmax(a->x_ac_w, a->x_cg), dmax(xnp, x_tail_ac));
    const double margin = 0.20; /* m */
    xmin -= margin;
    xmax += margin;

    /* SVG canvas */
    const int W = 900;
    const int H = 260;

    /* Map x [m] -> pixel */
    const double scale = (double)(W - 120) / (xmax - xmin); /* px per metre */
    const double x0 = 60; /* left padding */
    const double y_mid = 130;

    const double fus_y = y_mid;
    const double wing_y = y_mid - 20;

    /* Simple wing geometry on drawing */
    const double wing_span_px = 80;
    const double wing_chord_px = 40;

    /* Compute wing leading edge from x_ac_w assuming quarter-chord AC:
       x_le = x_ac_w - 0.25*c_bar
    */
    const double x_le = a->x_ac_w - 0.25 * a->c_bar;

    const double x_le_px = x0 + (x_le - xmin) * scale;
    const double x_ac_px = x0 + (a->x_ac_w - xmin) * scale;
    const double x_cg_px = x0 + (a->x_cg - xmin) * scale;
    const double x_np_px = x0 + (xnp - xmin) * scale;
    const double x_tail_px = x0 + (x_tail_ac - xmin) * scale;

    FILE *f = fopen(path, "w");
    if (!f) return 0;

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\">\n", W, H, W, H);
    fprintf(f, "<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" fill=\"white\"/>\n", W, H);

    /* Styles */
    fprintf(f, "<style>\n");
    fprintf(f, ".txt{font-family:Arial,Helvetica,sans-serif;font-size:13px;fill:#111;}\n");
    fprintf(f, ".thin{stroke:#111;stroke-width:2;fill:none;}\n");
    fprintf(f, ".dash{stroke:#111;stroke-width:2;fill:none;stroke-dasharray:6 6;}\n");
    fprintf(f, ".mark{stroke:#111;stroke-width:3;}\n");
    fprintf(f, "</style>\n");

    /* Fuselage line */
    fprintf(f, "<line class=\"thin\" x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\"/>\n", x0, fus_y, (double)(W - 60), fus_y);

    /* Wing rectangle */
    fprintf(f, "<rect x=\"%.1f\" y=\"%.1f\" width=\"%.1f\" height=\"%.1f\" fill=\"none\" stroke=\"#111\" stroke-width=\"2\"/>\n",
            x_le_px, wing_y - wing_chord_px/2.0, wing_span_px, wing_chord_px);

    /* Lift arrow (symbolic) above wing */
    fprintf(f, "<line class=\"thin\" x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\"/>\n",
            x_ac_px, wing_y - 65, x_ac_px, wing_y - 25);
    fprintf(f, "<polygon points=\"%.1f,%.1f %.1f,%.1f %.1f,%.1f\" fill=\"#111\"/>\n",
            x_ac_px, wing_y - 70, x_ac_px - 6, wing_y - 60, x_ac_px + 6, wing_y - 60);
    fprintf(f, "<text class=\"txt\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">Lift (L)</text>\n", x_ac_px, wing_y - 80);

    /* Markers: wing AC, CG, NP, tail AC */
    fprintf(f, "<line class=\"mark\" x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\"/>\n", x_ac_px, fus_y - 25, x_ac_px, fus_y + 25);
    fprintf(f, "<text class=\"txt\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">Wing AC</text>\n", x_ac_px, fus_y + 45);

    fprintf(f, "<line class=\"mark\" x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\"/>\n", x_cg_px, fus_y - 25, x_cg_px, fus_y + 25);
    fprintf(f, "<text class=\"txt\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">CG</text>\n", x_cg_px, fus_y - 35);

    fprintf(f, "<line class=\"dash\" x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\"/>\n", x_np_px, fus_y - 30, x_np_px, fus_y + 30);
    fprintf(f, "<text class=\"txt\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">Neutral Point</text>\n", x_np_px, fus_y + 65);

    fprintf(f, "<line class=\"mark\" x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\"/>\n", x_tail_px, fus_y - 25, x_tail_px, fus_y + 25);
    fprintf(f, "<text class=\"txt\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">Tail AC</text>\n", x_tail_px, fus_y + 45);

    /* Tail arm dimension (CG -> Tail AC) */
    const double dim_y = fus_y + 85;
    fprintf(f, "<line class=\"thin\" x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\"/>\n", x_cg_px, dim_y, x_tail_px, dim_y);
    /* arrowheads */
    fprintf(f, "<polygon points=\"%.1f,%.1f %.1f,%.1f %.1f,%.1f\" fill=\"#111\"/>\n",
            x_cg_px, dim_y, x_cg_px + 8, dim_y - 5, x_cg_px + 8, dim_y + 5);
    fprintf(f, "<polygon points=\"%.1f,%.1f %.1f,%.1f %.1f,%.1f\" fill=\"#111\"/>\n",
            x_tail_px, dim_y, x_tail_px - 8, dim_y - 5, x_tail_px - 8, dim_y + 5);
    fprintf(f, "<text class=\"txt\" x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">l_t = %.3f m</text>\n",
            (x_cg_px + x_tail_px)/2.0, dim_y - 10, a->l_t);

    /* Numeric summary */
    fprintf(f, "<text class=\"txt\" x=\"%d\" y=\"25\">x_cg = %.3f m, x_np = %.3f m, SM = %.2f%% MAC</text>\n",
            60, a->x_cg, xnp, static_margin(a) * 100.0);

    fprintf(f, "</svg>\n");
    fclose(f);
    return 1;
}

int write_wing_detail_svg(const char *path, const Aircraft *a)
{
    (void)a;
    FILE *f = fopen(path, "w");
    if (!f) return 0;

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"700\" height=\"220\" viewBox=\"0 0 700 220\">\n");
    fprintf(f, "<rect width=\"700\" height=\"220\" fill=\"white\"/>\n");
    fprintf(f, "<text x=\"20\" y=\"40\" font-family=\"Arial\" font-size=\"16\">Wing detail (TODO)</text>\n");
    fprintf(f, "</svg>\n");

    fclose(f);
    return 1;
}

int write_tail_detail_svg(const char *path, const Aircraft *a)
{
    (void)a;
    FILE *f = fopen(path, "w");
    if (!f) return 0;

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"700\" height=\"220\" viewBox=\"0 0 700 220\">\n");
    fprintf(f, "<rect width=\"700\" height=\"220\" fill=\"white\"/>\n");
    fprintf(f, "<text x=\"20\" y=\"40\" font-family=\"Arial\" font-size=\"16\">Tail detail (TODO)</text>\n");
    fprintf(f, "</svg>\n");

    fclose(f);
    return 1;
}