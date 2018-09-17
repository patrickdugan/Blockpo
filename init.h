/** Global variables for status*/
int N=2;
n_cols = 8;

pt_open_incr_long  = new VectorTL(N); VectorTL &open_incr_longv = *pt_open_incr_long;
open_incr_longv[0] = "OpenLongPosition";
open_incr_longv[1] = "LongPosIncreased";

pt_open_incr_short  = new VectorTL(N); VectorTL &open_incr_shortv = *pt_open_incr_short;
open_incr_shortv[0] = "OpenShortPosition";
open_incr_shortv[1] = "ShortPosIncreased";

pt_database = new MatrixTL(n_rows, n_cols); MatrixTL &database = *pt_database;
std::fstream fileg;
fileg.open("graphInfoSixth.txt", std::ios::in);
for (int i = 0; i < n_rows; ++i) for (int j = 0; j < n_cols; ++j) fileg >> database[i][j];
fileg.close();