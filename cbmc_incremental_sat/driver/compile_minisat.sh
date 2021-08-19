#rm icnf_driver
# cp /workspace/ipasir/sat/minisat220/libipasirminisat220.a .
#g++ -I. -o icnf_driver main.cpp -L. -lipasirminisat220
g++ -I. -o icnf_driver main.cpp -L../../ipasir/sat/minisat220/ -lipasirminisat220
#./icnf_driver ../cnfs/cbmc_cnf_2NCtXR 
#./icnf_driver ../cnfs/cbmc_cnf_pG438A
