#include "plot.h"

int main() {
    signalsmith::plot::Plot2D plot;

	// Customise the axes
	// plot.x.major(0).tick(10).label("time");
	// plot.y.major(0).minors(-1, 1).label("signal");

	// Add some data with `.add(x, y)`
	auto &line = plot.line();
	for (int x = 0; x <= 10; x += 1) {
        line.marker(x,x);
		line.add(x, x);
	}

    //add creates the lines
    //marker makes the points

	plot.write("example.svg");
}