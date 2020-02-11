#!/usr/bin/env bash

cd fitting

root -l << EOF
.L create_histograms.C++g
create_histograms()
EOF

root -l << EOF
.L fit_histograms.C++g
fit_histograms()
EOF

root -l draw_output_fit_histograms.C
