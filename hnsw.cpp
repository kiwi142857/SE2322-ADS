#include <iostream>
#include <fstream>
#include <thread>
#include "src/hnsw.hpp"
#include "util/util.hpp"
#include "util/vecs_io.hpp"
#include "util/ground_truth.hpp"
#include "util/parameter.hpp"

using namespace std;
using namespace HNSWLab;

HNSW hnsw;

void thread_query(int *query, int query_vec_dim, int gnd_vec_dim, std::vector<int>& result) {
    result = hnsw.query(query, gnd_vec_dim);
}

int main1() {
    
    std::printf("load ground truth\n");
    int gnd_n_vec = 100;
    int gnd_vec_dim = 10;
    char *path = "./data/siftsmall/gnd.ivecs";
    int *gnd = read_ivecs(gnd_n_vec, gnd_vec_dim, path);

    std::printf("load query\n");
    int query_n_vec = 100;
    int query_vec_dim = 128;
    path = "./data/siftsmall/query.bvecs";
    int *query = read_bvecs(query_n_vec, query_vec_dim, path);

    std::printf("load base\n");
    int base_n_vec = 10000;
    int base_vec_dim = 128;
    path = "./data/siftsmall/base.bvecs";
    int *base = read_bvecs(base_n_vec, base_vec_dim, path);

    hnsw.setDimension(base_vec_dim);

    size_t report_every = 1000;
    TimeRecord insert_record;

    for (int i = 0; i < base_n_vec; i++) {
        hnsw.insert(base + base_vec_dim * i, i);

        if (i % report_every == 0) {
            insert_record.reset();
        }
    }

    printf("querying\n");
    vector <vector<int>> test_gnd_l;
    double single_query_time;
    TimeRecord query_record;

    std::vector<std::thread> threads;
    test_gnd_l.resize(gnd_n_vec);
    if(isMultiThread){
        for (int i = 0; i < gnd_n_vec; i++) {
            threads.push_back(std::thread(thread_query, query + query_vec_dim * i, query_vec_dim, gnd_vec_dim, std::ref(test_gnd_l[i])));
        }
        for (auto& th : threads) {
            th.join();
        }
    } else {
        for (int i = 0; i < gnd_n_vec; i++) {
            test_gnd_l[i] = hnsw.query(query + query_vec_dim * i, gnd_vec_dim);
        }
    }
    single_query_time = query_record.get_elapsed_time_micro() / query_n_vec * 1e-3;

    printf("M: %d, M_max: %d, ef_construction: %d, ef_search: %d\n", M, M_max, ef_construction, ef_search);
    double recall = count_recall(gnd_n_vec, gnd_vec_dim, test_gnd_l, gnd);
    printf("average recall: %.3f, single query time %.1f ms\n", recall, single_query_time);
    return 0;
}

int SimpleTest(){
    HNSW hnsw(128);
    int vec1[128] = {1, 2, 3, 4, 5};
    int vec2[128] = {2, 3, 4, 5, 6};
    int vec3[128] = {3, 4, 5, 6, 7};
    int vec4[128] = {4, 5, 6, 7, 8};
    int vec5[128] = {5, 6, 7, 8, 9};
    int vec6[128] = {6, 7, 8, 9, 10};
    int vec7[128] = {7, 8, 9, 10, 11};
    int vec8[128] = {8, 9, 10, 11, 12};
    int vec9[128] = {9, 10, 11, 12, 13};
    int vec10[128] = {10, 11, 12, 13, 14};
    hnsw.insert(vec1, 1);
    hnsw.insert(vec2, 2);
    hnsw.insert(vec3, 3);
    hnsw.insert(vec4, 4);
    hnsw.insert(vec5, 5);
    hnsw.insert(vec6, 6);
    hnsw.insert(vec7, 7);
    hnsw.insert(vec8, 8);
    hnsw.insert(vec9, 9);
    hnsw.insert(vec10, 10);
    vector<int> res = hnsw.query(vec1, 5);
    for (int i = 0; i < res.size(); i++){
        cout << res[i] << " ";
    }
    cout << endl;
    return 0;
}

int testUtil(){
    for(int i=0;i<100;++i){
        cout << get_random_level() << endl;
    }
}
int main(){
    // SimpleTest();
    // testUtil();
    main1();
    return 0;
}