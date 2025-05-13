/***********************************************
 * socalc.c The Schedule-I drug mix calculator *
 * ******************************************* *
 * GPLv3 (C) 2025 ZnxTech                      *
 * https://github.com/ZnxTech/scheduleone_calc *
 ***********************************************/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define EFFECT_COUNT 35
#define MIXIN_COUNT  16
#define DRUG_COUNT   6

struct effect;
struct conversion;
struct mixin;
struct drug;

/************************
 * SCHEDULE ONE EFFECTS *
 ************************/

struct effect {
    const char *name;
    float multiplier;
};

const struct effect EFFECTS[EFFECT_COUNT] = {
    /* name:                multi:     id: */
    { "disorienting",       0.00f }, /*  0 */
    { "explosive",          0.00f }, /*  1 */
    { "laxative",           0.00f }, /*  2 */
    { "lethal",             0.00f }, /*  3 */
    { "paranoia",           0.00f }, /*  4 */
    { "schizophrenic",      0.00f }, /*  5 */
    { "seizure",            0.00f }, /*  6 */
    { "smelly",             0.00f }, /*  7 */
    { "toxic",              0.00f }, /*  8 */
    /* none zero start here. */
    { "calming",            0.10f }, /*  9 */
    { "munchies",           0.12f }, /* 10 */
    { "refreshing",         0.14f }, /* 11 */
    { "focused",            0.16f }, /* 12 */
    { "euphoric",           0.18f }, /* 13 */
    { "gingeritis",         0.20f }, /* 14 */
    { "energizing",         0.22f }, /* 15 */
    { "sneaky",             0.24f }, /* 16 */
    { "sedating",           0.26f }, /* 17 */
    { "caloriedense",       0.28f }, /* 18 */
    { "balding",            0.30f }, /* 19 */
    { "athletic",           0.32f }, /* 20 */
    { "slippery",           0.34f }, /* 21 */
    { "foggy",              0.36f }, /* 22 */
    { "spicy",              0.38f }, /* 23 */
    { "brighteyed",         0.40f }, /* 24 */
    { "jennerising",        0.42f }, /* 25 */
    { "thoughtprovoking",   0.44f }, /* 26 */
    { "tropicthunder",      0.46f }, /* 27 */
    { "glowie",             0.48f }, /* 28 */ /* glowing */
    { "electrifying",       0.50f }, /* 29 */
    { "giraffying",         0.52f }, /* 30 */ /* long faced */
    { "antigravity",        0.54f }, /* 31 */
    { "cyclopean",          0.56f }, /* 32 */
    { "zombifying",         0.58f }, /* 33 */
    { "shrinking",          0.60f }  /* 34 */
};

#define EFFECT_MASK(id)       ((uint64_t)1 << id)
#define EFFECT_HAS(field, id) (((uint64_t)field >> id) & 1)

/**********************************
 * SCHEDULE ONE MIXIN CONVERSIONS *
 **********************************/

struct conversion {
    uint8_t effect_from;
    uint8_t effect_to;
};

#define CONV_COUNT(conv) (sizeof(conv) / sizeof(struct conversion))

/**
 * the first id is the effect to be converted 
 * into the effect of the second id.
 * convesions are done from top to bottom, lower indexed first,
 * and will not convert if the resulting effect is already present.
 * comments are to track conversion chains which their ORDER MATTERS! 
 */
struct conversion CUKE_CONV[] = {
    { 10, 20 },
    { 22, 32 },
    { 21, 10 },
    { 16,  4 },
    { 14, 26 },
    {  8, 13 }, /* 0, 1st */
    { 13,  2 }  /* 0, 2nd */
};

struct conversion BANANA_CONV[] = {
    {  7, 31 },
    {  4, 25 },
    { 30, 11 },
    {  8,  7 },
    {  9, 16 },
    {  0, 12 }, /* 0, 1st */
    { 12,  6 }, /* 0, 2nd */
    { 15, 26 }, /* 1, 1st */
    { 32, 15 }  /* 1, 2nd */
};

struct conversion PARAC_CONV[] = {
    { 10, 31 },
    { 29, 20 },
    { 23, 24 },
    { 12, 14 },
    {  9, 21 }, /* 0, 1st */
    { 22,  9 }, /* 0, 2nd */
    { 26,  8 }, /* 1, 1st */
    {  8, 27 }, /* 1, 2nd */
    { 15,  4 }, /* 2, 1st */
    {  4, 19 }  /* 2, 2nd */
};

struct conversion DONUT_CONV[] = {
    { 34, 15 },
    { 12, 13 },
    { 18,  1 },
    { 25, 14 },
    { 31, 21 },
    { 19, 16 }
};

struct conversion VIAGRA_CONV[] = {
    { 13, 24 },
    {  2,  9 },
    { 20, 16 },
    {  0,  8 }
};

struct conversion MOUTH_WASH_CONV[] = {
    {  9, 31 },
    { 12, 25 },
    {  1, 17 },
    { 18, 16 }
};

struct conversion FLU_MED_CONV[] = {
    { 32, 22 },
    { 26, 14 },
    { 34,  4 },
    { 30, 11 },
    {  9, 24 }, /* 0, 1st */
    { 12,  9 }, /* 0, 2nd */
    { 20, 10 }, /* 1, 1st */
    { 10, 21 }, /* 1, 2nd */
    {  2, 13 }, /* 2, 1st */
    { 13,  8 }  /* 2, 2nd */
};

struct conversion GASOLINE_CONV[] = {
    {  4,  9 },
    { 34, 12 },
    {  2, 22 },
    { 10, 17 },
    { 14,  7 },
    {  0, 28 }, /* 0, 1st */
    { 29,  0 }, /* 0, 2nd */
    { 15, 13 }, /* 1, 1st */
    { 13, 23 }, /* 1, 2nd */
    { 25, 16 }, /* 2, 1st */
    { 16, 27 }  /* 2, 2nd */
};

struct conversion ENEGRY_DRINK_CONV[] = {
    {  5, 19 },
    { 22,  2 },
    { 17, 10 },
    { 12, 34 },
    { 27, 16 },
    { 13, 15 }, /* 0, 1st */
    { 23, 13 }, /* 0, 2nd */
    { 28,  0 }, /* 1, 1st */
    {  0, 29 }  /* 1, 2nd */
};

struct conversion MOTOR_OIL_CONV[] = {
    {  4, 31 },
    { 13, 17 },
    { 22,  8 },
    { 15, 10 }, /* 0, 1st */
    { 10,  5 }  /* 0, 2nd */
};

struct conversion BEAN_CONV[] = {
    { 34, 29 },
    { 20,  2 },
    { 25,  4 },
    { 21,  8 },
    {  6, 12 }, /* 0, 1st */
    { 12,  0 }, /* 0, 2nd */
    { 15, 32 }, /* 1, 1st */
    { 26, 15 }, /* 1, 2nd */
    {  9, 28 }, /* 2, 1st */
    { 16,  9 }  /* 2, 2nd */
};

struct conversion CHILI_CONV[] = {
    { 16, 24 },
    { 20, 13 },
    {  2, 30 },
    { 34, 11 },
    { 10,  8 },
    { 31, 27 }
};

struct conversion BATTERY_CONV[] = {
    {  2, 18 },
    { 32, 28 },
    { 34, 10 },
    { 31, 27 },
    { 13, 33 }, /* 0, 1st */
    { 29, 13 }  /* 0, 2nd */
};

struct conversion IODINE_CONV[] = {
    { 18, 14 },
    { 22,  4 },
    {  9, 17 },
    { 13,  6 },
    {  8, 16 },
    { 11, 26 }
};

struct conversion ADDY_CONV[] = {
    { 30, 29 },
    { 22, 15 },
    {  1, 13 },
    { 17, 14 },
    { 28, 11 }
};

struct conversion HORSE_SEMEN_CONV[] = {
    { 31,  9 },
    { 26, 29 },
    { 14, 11 }
};

/***********************
 * SCHEDULE ONE MIXINS *
 ***********************/

struct mixin {
    const char *name;
    uint32_t cost;
    uint8_t effect;
    uint8_t conv_count;
    struct conversion *convs;
};

const struct mixin MIXINS[] = {
    /* name:          cost: effect: count:                         conversions:           id: */
    { "cuke",         2,    15,     CONV_COUNT(CUKE_CONV),         CUKE_CONV         }, /*  0 */
    { "banana",       2,    14,     CONV_COUNT(BANANA_CONV),       BANANA_CONV       }, /*  1 */
    { "paracetamol",  3,    16,     CONV_COUNT(PARAC_CONV),        PARAC_CONV        }, /*  2 */
    { "donut",        3,    18,     CONV_COUNT(DONUT_CONV),        DONUT_CONV        }, /*  3 */
    { "viagra",       4,    27,     CONV_COUNT(VIAGRA_CONV),       VIAGRA_CONV       }, /*  4 */
    { "mouth_wash",   4,    19,     CONV_COUNT(MOUTH_WASH_CONV),   MOUTH_WASH_CONV   }, /*  5 */
    { "flu_medicine", 5,    17,     CONV_COUNT(FLU_MED_CONV),      FLU_MED_CONV      }, /*  6 */
    { "gasoline",     5,     8,     CONV_COUNT(GASOLINE_CONV),     GASOLINE_CONV     }, /*  7 */
    { "energy_drink", 6,    20,     CONV_COUNT(ENEGRY_DRINK_CONV), ENEGRY_DRINK_CONV }, /*  8 */
    { "motor_oil",    6,    20,     CONV_COUNT(MOTOR_OIL_CONV),    MOTOR_OIL_CONV    }, /*  9 */
    { "mega_bean",    7,    22,     CONV_COUNT(BEAN_CONV),         BEAN_CONV         }, /* 10 */
    { "chili",        7,    23,     CONV_COUNT(CHILI_CONV),        CHILI_CONV        }, /* 11 */
    { "battery",      8,    24,     CONV_COUNT(BATTERY_CONV),      BATTERY_CONV      }, /* 12 */
    { "iodine",       8,    25,     CONV_COUNT(IODINE_CONV),       IODINE_CONV       }, /* 13 */
    { "addy",         9,    26,     CONV_COUNT(ADDY_CONV),         ADDY_CONV         }, /* 14 */
    { "horse_semen",  9,    30,     CONV_COUNT(HORSE_SEMEN_CONV),  HORSE_SEMEN_CONV  }  /* 15 */ 
};

/*********************************************
 * MIXIN DYNAMIC ARRAY (for recipe tracking) *
 *********************************************/

struct mixin_darray {
    struct mixin *array;
    uint32_t count;
    uint32_t reserve;
};

struct mixin_darray *mixin_darray_create(void)
{
    struct mixin_darray *darray;
    darray = malloc(sizeof(struct mixin_darray));
    darray->array = malloc(sizeof(struct mixin));
    darray->count = 0;
    darray->reserve = 1;
    return darray;
}

void mixin_darray_reserve(struct mixin_darray *darray, size_t reserve)
{
    darray->array = realloc(darray->array, reserve * sizeof(struct mixin));
    darray->reserve = reserve;
}

void mixin_darray_append(struct mixin_darray *darray, struct mixin mixin)
{
    if (darray->count == darray->reserve)
        mixin_darray_reserve(darray, darray->reserve * 2);

    darray->array[darray->count] = mixin;
    darray->count++;
}

struct mixin_darray *mixin_darray_clone(struct mixin_darray *darray)
{
    if (darray == NULL) /* NULL darray treated as an clean empty darray. */
        return mixin_darray_create();
    
    struct mixin_darray *clone;
    clone = malloc(sizeof(struct mixin_darray));

    /* memcpy the darray struct. */
    memcpy(clone, darray, sizeof(struct mixin_darray));

    /* memcpy the darray array. */
    const size_t array_size = darray->reserve * sizeof(struct mixin);
    clone->array = malloc(array_size);
    memcpy(clone->array, darray->array, array_size);
    return clone;
}

void mixin_darray_free(struct mixin_darray *darray)
{
    free(darray->array);
    free(darray);
}

/**********************
 * SCHEDULE ONE DRUGS *
 **********************/

struct drug {
    const char *name;
    uint32_t cost;
    uint32_t value;
    uint64_t effects;
    struct mixin_darray *mixins;
};

const struct drug DRUGS[DRUG_COUNT] = {
    { "og_kush",          0, 35,  EFFECT_MASK(9),  NULL },
    { "sour_diesel",      0, 35,  EFFECT_MASK(11), NULL },
    { "green_crack",      0, 35,  EFFECT_MASK(15), NULL },
    { "grandaddy_purple", 0, 35,  EFFECT_MASK(17), NULL },
    { "methamphetamine",  0, 70,  0, /*no effect*/ NULL },
    { "cocaine",          0, 150, 0, /*no effect*/ NULL }
};

float drug_calc_multiplier(struct drug drug)
{
    float multiplier = 1.0f;
    for (uint32_t i_effect = 0; i_effect < EFFECT_COUNT; i_effect++ ) {
        if (EFFECT_HAS(drug.effects, i_effect))
            multiplier += EFFECTS[i_effect].multiplier;
    }

    return multiplier;
}

uint32_t drug_calc_value(struct drug drug, float multiplier)
{
    return (uint32_t)(drug.value * multiplier);
}

int32_t drug_calc_profit(struct drug drug, float multiplier)
{
    return (int32_t)drug_calc_value(drug, multiplier) - (int32_t)drug.cost;
}

inline bool drug_can_apply_conv(struct drug *drug, struct conversion conv,
                                uint64_t del_mask)
{
    return EFFECT_HAS(drug->effects, conv.effect_from) 
        && (!EFFECT_HAS(drug->effects, conv.effect_to)
        || EFFECT_HAS(del_mask, conv.effect_to));
}

uint32_t drug_get_effect_number(struct drug *drug)
{
    uint32_t effect_number = 0;
    for (uint32_t i = 0; i < EFFECT_COUNT; i++) {
        effect_number += (drug->effects & EFFECT_MASK(i)) != 0;
    }
    return effect_number;
}

void drug_apply_mixin(struct drug *drug, struct mixin mixin)
{
    if (drug->mixins == NULL)
        drug->mixins = mixin_darray_create();

    /* apply all of the mixin's effect conversions. */
    uint64_t del_mask = 0;
    uint64_t add_mask = 0;
    for (uint32_t i_conv = 0; i_conv < mixin.conv_count; i_conv++ ) {
        struct conversion conv = mixin.convs[i_conv];
        if (drug_can_apply_conv(drug, conv, del_mask)) {
            del_mask += EFFECT_MASK(conv.effect_from);
            add_mask += EFFECT_MASK(conv.effect_to);
        }
    }

    drug->effects &= ~del_mask;
    drug->effects |= add_mask;

    /* add the mixin's main effect. */
    if (drug_get_effect_number(drug) < 8) {
        drug->effects |= EFFECT_MASK(mixin.effect);
        drug->cost += mixin.cost;
    }
    mixin_darray_append(drug->mixins, mixin);
}

/***********************
 * BEST DRUGS RECORDER *
 ***********************/

struct recorder {
    bool has_drug[DRUG_COUNT];
    struct drug best_drugs[DRUG_COUNT];
    int32_t best_profit[DRUG_COUNT];
};

/** 
 * global recorder struct to
 * keep track of the best profit mixin recipes.
 */
struct recorder RECORDER = { 0 };

void drug_record(struct drug drug, uint32_t drug_id)
{
    float cont_multiplier = drug_calc_multiplier(drug);
    int32_t cont_profit = drug_calc_profit(drug, cont_multiplier);

    if (!RECORDER.has_drug[drug_id]) {
        struct drug init_drug = drug;
        init_drug.mixins = mixin_darray_clone(drug.mixins);
        RECORDER.best_profit[drug_id] = cont_profit;
        RECORDER.best_drugs[drug_id] = init_drug;
        RECORDER.has_drug[drug_id] = true;
        return;
    }

    if (RECORDER.best_profit[drug_id] < cont_profit) {
        struct drug best_drug = drug;
        best_drug.mixins = mixin_darray_clone(drug.mixins);
        mixin_darray_free(RECORDER.best_drugs[drug_id].mixins);
        RECORDER.best_profit[drug_id] = cont_profit;
        RECORDER.best_drugs[drug_id] = best_drug;
    }
}

void recorder_cleanup()
{
    for (uint32_t i_drug = 0; i_drug < DRUG_COUNT; i_drug++) {
        if (RECORDER.has_drug[i_drug]
        && RECORDER.best_drugs[i_drug].mixins != NULL)
            mixin_darray_free(RECORDER.best_drugs[i_drug].mixins);
    }
}

/************************
 * UGLY PRINT FUNCTIONS *
 ************************/

void print_devider(void)
{
    printf("+ -----------\n");
}

void print_drug_base(struct drug drug)
{
    printf("| Name:       %s\n", drug.name);
    printf("| Base value: $%u\n", drug.value);
}

#define MAX_EFFECT_LEN 80
void print_drug_effects(struct drug drug)
{
    printf("| Effects:\n");
    printf("| ");

    uint32_t cur_len = 0;
    for (uint32_t i_effect = 0; i_effect < EFFECT_COUNT; i_effect++) {
        if (!EFFECT_HAS(drug.effects, i_effect))
            continue;

        struct effect effect = EFFECTS[i_effect];
        cur_len += snprintf(NULL, 0, "[%s x%.2f] ", effect.name, effect.multiplier);
        if (cur_len > MAX_EFFECT_LEN) {
            printf("\n| ");
            cur_len = 0;
        }

        printf("[%s x%.2f] ", effect.name, effect.multiplier);
    }

    printf("\n");
}

void print_drug_multiplier(struct drug drug)
{
    float multiplier = drug_calc_multiplier(drug);
    printf("| Multiplier: x%.2f\n", multiplier);
    printf("| Final value: $%u\n", drug_calc_value(drug, multiplier));
}

void print_drug_cost(struct drug drug)
{
    printf("| Cost: $%u\n", drug.cost);
}

#define MAX_RECIPE_LEN 80
void print_drug_recipe(struct drug drug)
{
    if (drug.mixins == NULL)
        return;

    printf("| Recipe:\n");
    printf("| ");

    uint32_t cur_len = 0;
    for (uint32_t i_mixin = 0; i_mixin < drug.mixins->count; i_mixin++) {
        struct mixin mixin = drug.mixins->array[i_mixin];
        cur_len += snprintf(NULL, 0, "[%s $%u] -> ", mixin.name, mixin.cost);
        if (cur_len > MAX_RECIPE_LEN) {
            printf("\n| ");
            cur_len = 0;
        }

        printf("[%s $%u] -> ", mixin.name, mixin.cost);
    }

    printf("Done!\n");
}

void print_drug(struct drug drug)
{
    print_devider();
    printf("| Drug Info:\n");
    print_devider();
    print_drug_base(drug);
    print_devider();
    print_drug_effects(drug);
    print_drug_multiplier(drug);
    print_devider();
    print_drug_cost(drug);
    print_drug_recipe(drug);
    print_devider();
}

void list_print_drugs()
{
    printf("+ ---- + ----------------- + ------ +\n");
    printf("| ids: | names:            | value: |\n");
    printf("+ ---- + ----------------- + ------ +\n");
    for (uint32_t i_drug = 0; i_drug < DRUG_COUNT; i_drug++) {
        struct drug drug = DRUGS[i_drug];
        printf("| %-4u | %-17s | $%-5u |\n", i_drug, drug.name, drug.value);
    }
    printf("+ ---- + ----------------- + ------ +\n");
}

void list_print_effects()
{
    printf("+ ---- + ----------------- + ------------ +\n");
    printf("| ids: | names:            | multipliers: |\n");
    printf("+ ---- + ----------------- + ------------ +\n");
    for (uint32_t i_effect = 0; i_effect < EFFECT_COUNT; i_effect++) {
        struct effect effect = EFFECTS[i_effect];
        printf("| %-4u | %-17s | x%-11.2f |\n", i_effect, effect.name, effect.multiplier);
    }
    printf("+ ---- + ----------------- + ------------ +\n");
}

void list_print_mixins()
{
    printf("+ ---- + -------------- + ------ +\n");
    printf("| ids: | names:         | costs: |\n");
    printf("+ ---- + -------------- + ------ +\n");
    for (uint32_t i_mixin = 0; i_mixin < MIXIN_COUNT; i_mixin++) {
        struct mixin mixin = MIXINS[i_mixin];
        printf("| %-4u | %-14s | $%-5u |\n", i_mixin, mixin.name, mixin.cost);
    }
    printf("+ ---- + -------------- + ------ +\n");
}

/**************************
 * DRUG MIX BRUTE-FORCING *
 **************************/

void drug_recursive_mix(uint32_t mixin_depth, struct drug drug, uint32_t drug_id)
{
    mixin_depth--;
    for (uint32_t i_mixin = 0; i_mixin < MIXIN_COUNT; i_mixin++) {
        struct drug drug_clone = drug;
        drug_clone.mixins = mixin_darray_clone(drug.mixins);
        drug_apply_mixin(&drug_clone, MIXINS[i_mixin]);
        drug_record(drug_clone, drug_id);
        if (mixin_depth > 0)
            drug_recursive_mix(mixin_depth, drug_clone, drug_id);

        mixin_darray_free(drug_clone.mixins);
    }
}

void drug_brute(uint32_t mixin_depth, uint32_t drug_id)
{
    struct drug drug = DRUGS[drug_id];
    if (drug.mixins == NULL)
        drug.mixins = mixin_darray_create();

    drug_recursive_mix(mixin_depth, drug, drug_id);
    mixin_darray_free(drug.mixins);

    if (RECORDER.has_drug[drug_id])
        print_drug(RECORDER.best_drugs[drug_id]);
}
 
void drug_brute_all(uint32_t mixin_depth)
{
    for (uint32_t i_drug = 0; i_drug < DRUG_COUNT; i_drug++)
        drug_brute(mixin_depth, i_drug);
}

/*****************************
 * SOCALC BASE PROG HANDLING *
 *****************************/

const char *HELP = "\
The Schedule-I Drug Mix Calculator.                                            \n\
Usage: socalc [options...]                                                     \n\
                                                                               \n\
  -h, --help                   Prints this help message.                       \n\
  -l, --list <type>            Prints a list of all <type>.                    \n\
  -b, --brute <drug> <max=4>   Brute-force the best profit mix for <drug>.     \n\
  -a, --brute-all <max=4>      Brute-force the best profit mix for all drugs.  \n\
  -r, --recipe <drug> <mixins> Calculates the resulting drug from <drug> and a-\n\
                               list of <mixins>.                               \n\
                                                                               \n\
  <type>:   [drugs|effects|mixins]                                             \n\
  <drug>:   [og_kush|sour_diesel|...(names listed on -l drugs)...]             \n\
  <mixins>: Space separated list of mixins name (names listed on -l mixins).   \n\
  <max>:    Max number of mixins to brute-force with, must be bigger than 0.   \n";

void handle_list_arg(int argc, char **argv)
{
    if (argc < 3) {
        printf("No list type provided.\n");
        return;
    }

    if (strcmp(argv[2], "drugs") == 0)
        list_print_drugs();
    else
    if (strcmp(argv[2], "effects") == 0)
        list_print_effects();
    else
    if (strcmp(argv[2], "mixins") == 0)
        list_print_mixins();
    else
        printf("No valid list type provided.\n");
}

bool drug_id_from_name(uint32_t *r_drug_id, const char *drug_name)
{
    for (uint32_t i_drug = 0; i_drug < DRUG_COUNT; i_drug++) {
        if (strcmp(drug_name, DRUGS[i_drug].name) == 0) {
            *r_drug_id = i_drug;
            return true;
        }
    }

    return false;
}

void handle_brute_arg(int argc, char **argv)
{
    /* obtain drug id. */
    if (argc < 3) {
        printf("No drug name provided.\n");
        return;
    }
    
    uint32_t drug_id;
    bool valid_drug = drug_id_from_name(&drug_id, argv[2]);
    if (!valid_drug) {
        printf("No valid drug name provided.\n");
        return;
    }

    /* obtain brute-force depth. */
    unsigned long mixin_depth = 4;
    if (argc > 3)
        mixin_depth = strtoul(argv[3], NULL, 10);

    if (mixin_depth > 100) {
        printf("Are you crazy?\n");
        return;
    } else if (mixin_depth == 0) {
        printf("No valid mixin depth provided.\n");
        return;
    }

    printf("Brute-forcing %s...\n", DRUGS[drug_id].name);
    drug_brute((uint32_t)mixin_depth, drug_id);
}

void handle_brute_all_arg(int argc, char **argv)
{
    /* obtain brute-force depth. */
    unsigned long mixin_depth = 4;
    if (argc > 2)
        mixin_depth = strtoul(argv[2], NULL, 10);

    if (mixin_depth > 100) {
        printf("Are you crazy?\n");
        return;
    } else if (mixin_depth == 0) {
        printf("No valid mixin depth provided.\n");
        return;
    }

    printf("Brute-forcing all drugs...\n");
    drug_brute_all((uint32_t)mixin_depth);
}

bool mixin_id_from_name(uint32_t *r_mixin_id, const char *mixin_name)
{
    for (uint32_t i_mixin = 0; i_mixin < MIXIN_COUNT; i_mixin++) {
        if (strcmp(mixin_name, MIXINS[i_mixin].name) == 0) {
            *r_mixin_id = i_mixin;
            return true;
        }
    }

    return false;
}

void handle_recipe_arg(int argc, char **argv)
{
    /* obtain drug id. */
    if (argc < 3) {
        printf("No drug name provided.\n");
        return;
    }
    
    uint32_t drug_id;
    bool valid_drug = drug_id_from_name(&drug_id, argv[2]);
    if (!valid_drug) {
        printf("No valid drug name provided.\n");
        return;
    }

    struct drug drug = DRUGS[drug_id];
    for (uint32_t i_arg = 3; i_arg < argc; i_arg++) {
        uint32_t mixin_id;
        bool valid_mixin = mixin_id_from_name(&mixin_id, argv[i_arg]);
        if (!valid_mixin) {
            printf("Arg %u is not a valid mixin name.\n", i_arg);
            return;
        }

        drug_apply_mixin(&drug, MIXINS[mixin_id]);
    }

    print_drug(drug);
}

void handle_args(int argc, char **argv)
{
    if (argc < 2) {
        printf(HELP);
        return;
    }
    
    if (strcmp(argv[1], "-h") == 0
    || strcmp(argv[1], "--help") == 0)
        printf(HELP);
    else
    if (strcmp(argv[1], "-l") == 0
    || strcmp(argv[1], "--list") == 0)
        handle_list_arg(argc, argv);
    else
    if (strcmp(argv[1], "-b") == 0
    || strcmp(argv[1], "--brute") == 0)
        handle_brute_arg(argc, argv);
    else
    if (strcmp(argv[1], "-a") == 0
    || strcmp(argv[1], "--brute-all") == 0)
        handle_brute_all_arg(argc, argv);
    else
    if (strcmp(argv[1], "-r") == 0
    || strcmp(argv[1], "--recipe") == 0)
        handle_recipe_arg(argc, argv);
}

int main(int argc, char **argv)
{
    handle_args(argc, argv);
    recorder_cleanup();
    return 0;
}
