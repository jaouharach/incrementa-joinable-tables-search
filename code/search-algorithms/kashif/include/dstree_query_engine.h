//
//  dstree_query_engine.h
//  ds-tree C version
//
//  Created by Karima Echihabi on 18/12/2016
//  Copyright 2012 Paris Descartes University. All rights reserved.
//
//  Based on isax code written by Zoumpatianos on 3/12/12.
//  Copyright 2012 University of Trento. All rights reserved.
//

#ifndef al_dstree_dstree_query_engine_h
#define al_dstree_dstree_query_engine_h
#include "../config.h"
#include "../globals.h"

#include "dstree_index.h"

#include "dstree_node.h"
#include "pqueue.h"

typedef struct query_result {
  ts_type distance;
  struct dstree_node *node;
  ts_type max_distance;
  size_t pqueue_position;
  // unsigned char label;
  label_type label;
  unsigned int file_pos;
  // ts_type *raw_series;

  /* start kashif changes */
  unsigned int query_vector_pos; // position of the query vector in the query set 
  struct vid *vector_id;
  double time;
  unsigned int num_checked_vectors;
  /* end kashif changes */
};

typedef struct bsf_snapshot {
  ts_type distance;
  double time;
  double cpu_time;
  ts_type *series;
  unsigned long checked_nodes;
  // unsigned int pos;
  // unsigned char label;
  label_type label;
  unsigned int file_pos;
  ////ts_type *raw_series;

  /* start kashif changes */
  unsigned int query_vector_pos;
  struct vid * vector_id;
  /* end kashif changes */
};

/// Data structure for sorting the query.
typedef struct q_index {
  double value;
  int index;
} q_index;

static int cmp_pri(double next, double curr) { return (next > curr); }

static double get_pri(void *a) {
  return (double)((struct query_result *)a)->distance;
}

static double get_max_pri(void *a) {
  return (double)((struct query_result *)a)->max_distance;
}

static void set_pri(void *a, double pri) {
  ((struct query_result *)a)->distance = (float)pri;
}

static void set_max_pri(void *a, double pri) {
  ((struct query_result *)a)->max_distance = (float)pri;
}

static size_t get_pos(void *a) {
  return ((struct query_result *)a)->pqueue_position;
}

static void set_pos(void *a, size_t pos) {
  ((struct query_result *)a)->pqueue_position = pos;
}

// struct query_result approximate_search (ts_type *ts, struct dstree_index
// *index); struct query_result exact_search (ts_type *ts, struct dstree_index
// *index,ts_type minimum_distance);

struct query_result exact_search(ts_type *query_ts, ts_type *query_reordered,
                                 int *query_order, unsigned int offset,
                                 struct dstree_index *index,
                                 ts_type minimum_distance, ts_type epsilon,
                                 ts_type delta);

struct query_result approximate_search(ts_type *query_ts,
                                       ts_type *query_ts_reordered,
                                       int *query_order, unsigned int offset,
                                       ts_type bsf, struct dstree_index *index);

void approximate_knn_search(ts_type *query_ts, ts_type *query_ts_reordered,
                            int *query_order, unsigned int offset, ts_type bsf,
                            struct dstree_index *index,
                            struct query_result *knn_results, unsigned int k,
                            struct bsf_snapshot **bsf_snapshots,
                            unsigned int *cur_bsf_snapshot,
                            unsigned int *curr_size, float warping);

void dstree_calc_tlb(ts_type *query_ts, struct dstree_index *index,
                     struct dstree_node *curr_node);

void update_query_stats(struct dstree_index *index, unsigned int query_id,
                        unsigned int found_knn, struct query_result bsf_result);
void print_query_stats(struct dstree_index *index, unsigned int query_num,
                       unsigned int found_knn, char *queries);
void get_query_stats(struct dstree_index *index, unsigned int found_knn);
void print_bsf_snapshots(struct dstree_index *index, unsigned int query_num,
                         unsigned int k, char *queries,
                         struct bsf_snapshot **bsf_snapshots,
                         unsigned int cur_bsf_snapshot);

void exact_knn_search_track_bsf(ts_type *query_ts, ts_type *query_ts_reordered,
                                int *query_order, unsigned int offset,
                                struct dstree_index *index,
                                ts_type minimum_distance, ts_type epsilon,
                                ts_type delta, unsigned int k,
                                unsigned int q_id, char *qfilename,
                                struct bsf_snapshot **bsf_snapshots,
                                unsigned int *cur_bsf_snapshot);
void exact_knn_search_track_pruning(ts_type *query_ts,
                                    ts_type *query_ts_reordered,
                                    int *query_order, unsigned int offset,
                                    struct dstree_index *index,
                                    ts_type minimum_distance, ts_type epsilon,
                                    ts_type delta, unsigned int k,
                                    unsigned int q_id, char *qfilename);

void exact_knn_search_max_policy(ts_type *query_ts, ts_type *query_ts_reordered,
                                 int *query_order, unsigned int offset,
                                 struct dstree_index *index,
                                 ts_type minimum_distance, ts_type epsilon,
                                 ts_type delta, unsigned int k,
                                 unsigned int q_id, char *qfilename);

void print_pruning_snapshots(struct dstree_node *node, ts_type node_bsf,
                             ts_type node_mindist, unsigned int k,
                             unsigned int query_num, char *queries);

void dump_mindists(struct dstree_index *index, struct dstree_node *node,
                   ts_type *query_ts);
ts_type get_node_QoS(struct dstree_index *index, struct dstree_node *node);

struct query_result * exact_de_knn_search(ts_type *query_ts, ts_type *query_ts_reordered,
                         int *query_order, unsigned int offset,
                         struct dstree_index *index, ts_type minimum_distance,
                         ts_type epsilon, ts_type r_delta, unsigned int k,
                         unsigned int q_id, char *qfilename);

void exact_ng_knn_search(ts_type *query_ts, ts_type *query_ts_reordered,
                         int *query_order, unsigned int offset,
                         struct dstree_index *index, ts_type minimum_distance,
                         unsigned int k, unsigned int q_id, char *qfilename,
                         unsigned int nprobes);

void exact_incr_knn_search(ts_type *query_ts, ts_type *query_ts_reordered,
                           int *query_order, unsigned int offset,
                           struct dstree_index *index, ts_type minimum_distance,
                           ts_type epsilon, ts_type r_delta, unsigned int k,
                           unsigned int q_id, char *qfilename,
                           unsigned int nprobes);
void print_perk_progressive_bsf_snapshots(struct dstree_index *index,
                                          unsigned int query_num,
                                          unsigned int found_knn, char *queries,
                                          struct bsf_snapshot **bsf_snapshots,
                                          unsigned int cur_bsf_snapshot,
                                          ts_type exact_distance, FILE *,
                                          FILE *, ts_type *);
void exact_de_incr_progressive_knn_search(
    ts_type *query_ts, ts_type *query_ts_reordered, int *query_order,
    unsigned int offset, struct dstree_index *index, ts_type minimum_distance,
    ts_type epsilon, ts_type r_delta, unsigned int k, unsigned int q_id,
    char *qfilename, struct bsf_snapshot **bsf_snapshots,
    unsigned int *cur_bsf_snapshot, float warping, FILE *, FILE *);

/* start kashif changes */
void approximate_knn_search_2(ts_type *query_ts, ts_type *query_ts_reordered,
                            int *query_order, unsigned int offset, ts_type bsf,
                            struct dstree_index *index,
                            struct query_result *knn_results, unsigned int k,
                            struct bsf_snapshot **bsf_snapshots,
                            unsigned int *cur_bsf_snapshot,
                            unsigned int *curr_size, float warping, struct vid * query_id, double * total_query_set_time, unsigned int * total_checked_ts);
void exact_de_progressive_knn_search(
    ts_type *query_ts, ts_type *query_ts_reordered, int *query_order,
    unsigned int offset, struct dstree_index *index, ts_type minimum_distance,
    ts_type epsilon, ts_type r_delta, unsigned int k, unsigned int q_id,
    char *qfilename, struct bsf_snapshot **bsf_snapshots,
    unsigned int *cur_bsf_snapshot);

struct query_result * exact_de_knn_search_2(ts_type *query_ts, ts_type *query_ts_reordered,
                         int *query_order, unsigned int offset,
                         struct dstree_index *index, ts_type minimum_distance,
                         ts_type epsilon, ts_type r_delta, unsigned int k,
                         unsigned int q_id, char *qfilename, double *total_query_set_time,
						 unsigned int *total_checked_ts, unsigned int query_vector_pos);
             
struct query_result *exact_de_incr_progressive_knn_search_2(
    ts_type *query_ts, ts_type *query_ts_reordered, int *query_order,
    unsigned int offset, struct dstree_index *index, ts_type minimum_distance,
    ts_type epsilon, ts_type r_delta, unsigned int k, unsigned int q_id,
    char *qfilename, double *total_query_set_time,
    unsigned int *total_checked_ts, struct bsf_snapshot **bsf_snapshots,
    unsigned int *cur_bsf_snapshot, float warping, FILE *dataset_file,
    FILE *series_file,struct vid * query_id);

struct query_result * exact_de_progressive_knn_search_2(
    ts_type *query_ts, ts_type *query_ts_reordered, int *query_order,
    unsigned int offset, struct dstree_index *index, ts_type minimum_distance,
    ts_type epsilon, ts_type r_delta, unsigned int k, unsigned int q_id,
    char *qfilename, double *total_query_set_time, unsigned int *total_checked_ts,
    struct bsf_snapshot **bsf_snapshots,
    unsigned int *cur_bsf_snapshot, unsigned int query_vector_pos);
/* end kashif changes */

#endif
