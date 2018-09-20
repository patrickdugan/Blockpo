/** Global variables and Status*/
int N = 2;
int M = 4;
n_cols = 8;
cols_news = n_cols + 2;

pt_open_incr_long  = new VectorTL(N); VectorTL &open_incr_longv = *pt_open_incr_long;
open_incr_longv[0] = "OpenLongPosition";
open_incr_longv[1] = "LongPosIncreased";

pt_open_incr_short  = new VectorTL(N); VectorTL &open_incr_shortv = *pt_open_incr_short;
open_incr_shortv[0] = "OpenShortPosition";
open_incr_shortv[1] = "ShortPosIncreased";

pt_netted_npartly_long  = new VectorTL(N); VectorTL &netted_npartly_long = *pt_netted_npartly_long;
netted_npartly_long[0] = "LongPosNetted";
netted_npartly_long[1] = "LongPosNettedPartly";

pt_netted_npartly_short  = new VectorTL(N); VectorTL &netted_npartly_short = *pt_netted_npartly_short;
netted_npartly_short[0] = "ShortPosNetted";
netted_npartly_short[1] = "ShortPosNettedPartly";

pt_open_incr_anypos = new VectorTL(M); VectorTL &open_incr_anypos = *pt_open_incr_anypos;
open_incr_anypos[0] = "OpenLongPosition";
open_incr_anypos[1] = "LongPosIncreased";
open_incr_anypos[2] = "OpenShortPosition";
open_incr_anypos[3] = "ShortPosIncreased";

pt_netted_npartly_anypos = new VectorTL(M); VectorTL &netted_npartly_anypos = *pt_netted_npartly_anypos;
netted_npartly_anypos[0] = "LongPosNetted";
netted_npartly_anypos[1] = "LongPosNettedPartly";
netted_npartly_anypos[2] = "ShortPosNetted";
netted_npartly_anypos[3] = "ShortPosNettedPartly";

pt_database = new MatrixTL(n_rows, n_cols); MatrixTL &database = *pt_database;
std::fstream fileg;
fileg.open("graphInfoSixth.txt", std::ios::in);
for (int i = 0; i < n_rows; ++i) for (int j = 0; j < n_cols; ++j) fileg >> database[i][j];
fileg.close();
