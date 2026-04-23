#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_AUTOMOBILI 10000
#define MAX_STRING 32
#define MAX_TARGA 6

struct Automobile {
  char marca[MAX_STRING];
  char modello[MAX_STRING];
  int cilindrata;
  float lunghezza;
  float peso;
  char targa[MAX_STRING];
};

struct Voce {
  char targa[MAX_TARGA]; // 5 lettere + terminatore
  int numero;
};

void sort(struct Voce *voci, int num_voci) {
  int i, j;
  struct Voce temp;

  for (i = 0; i < num_voci - 1; i++) {
    for (j = i + 1; j < num_voci; j++) {
      int cmp = strcmp(voci[i].targa, voci[j].targa);

      if (cmp > 0 || (cmp == 0 && voci[i].numero > voci[j].numero)) {
        // Scambia le posizioni delle voci
        temp = voci[i];
        voci[i] = voci[j];
        voci[j] = temp;
      }
    }
  }
}

// Converte le lettere della targa in cifre per tutto l'array
void swapCharacters(struct Voce *voci, int num_voci) {
  for (int v = 0; v < num_voci; v++) {
    for (int i = 0; i < (int)strlen(voci[v].targa); i++) {
      if (voci[v].targa[i] >= 'A' && voci[v].targa[i] <= 'Z') {
        voci[v].targa[i] = voci[v].targa[i] - 'A' + 1 + '0';
      }
    }
  }
}

// Inverte i caratteri della targa per tutto l'array
void reverseCharacters(struct Voce *voci, int num_voci) {
  for (int v = 0; v < num_voci; v++) {
    int length = strlen(voci[v].targa);
    for (int i = 0; i < length / 2; i++) {
      char temp = voci[v].targa[i];
      voci[v].targa[i] = voci[v].targa[length - i - 1];
      voci[v].targa[length - i - 1] = temp;
    }
  }
}

void randomize_car_data(char marca[MAX_STRING], char modello[MAX_STRING],
                        int *cilindrata, float *lunghezza, float *peso,
                        char targa[MAX_STRING]) {
  const char *brands[] = {"Ford",      "Toyota", "Honda",
                          "Chevrolet", "Nissan", "Volkswagen"};
  const char *models[] = {"Sedan",     "SUV",       "Truck",
                          "Hatchback", "Crossover", "Convertible"};

  strcpy(marca, brands[rand() % 6]);
  strcpy(modello, models[rand() % 6]);

  *cilindrata = (rand() % 6000) + 1000;
  *lunghezza = ((float)rand() / RAND_MAX) * (5.5 - 3.5) + 3.5; // era senza assegnazione
  *peso = ((float)rand() / RAND_MAX) * (2500.0 - 1000.0) + 1000.0;
  char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for (int i = 0; i < MAX_TARGA - 1; i++) {
    targa[i] = alphabet[rand() % 26];
  }
  targa[MAX_TARGA - 1] = '\0'; // terminatore nella posizione giusta
}

void write_car_data_to_file(const char *nomeFile, const char *nomeFileIndice,
                            int num_cars) {
  FILE *file = fopen(nomeFile, "w");
  if (file == NULL) {
    printf("Error opening file for writing.\n");
    return;
  }
  FILE *fileInd = fopen(nomeFileIndice, "w");
  if (fileInd == NULL) { // era: if (file == NULL), controllava il file sbagliato
    printf("Error opening file for writing.\n");
    return;
  }

  for (int i = 0; i < num_cars; i++) {
    char marca[MAX_STRING];
    char modello[MAX_STRING];
    int cilindrata;
    float lunghezza;
    float peso;
    char targa[MAX_STRING];

    randomize_car_data(marca, modello, &cilindrata, &lunghezza, &peso, targa);

    fprintf(file, "%s ", marca);
    fprintf(file, "%s ", modello);
    fprintf(file, "%d ", cilindrata);
    fprintf(file, "%.2f ", lunghezza);
    fprintf(file, "%.2f ", peso);
    fprintf(file, "%s\n", targa);

    fprintf(fileInd, "%s ", targa);
    fprintf(fileInd, "%d\n", i + 1);
  }

  fclose(fileInd);
  fclose(file);
}

void leggiDaFileTestoIndice(struct Voce voce[], const char *nomeFileInd) {
  FILE *fp = fopen(nomeFileInd, "r");
  if (fp == NULL) {
    printf("Errore nell'apertura del file di testo.\n");
    exit(1);
  }

  for (int i = 0; i < MAX_AUTOMOBILI; i++) {
    fscanf(fp, "%s %d", voce[i].targa, &voce[i].numero);
  }

  fclose(fp);
}

void leggiDaFileTesto(struct Automobile automobili[], const char *nomeFile) {
  FILE *fp = fopen(nomeFile, "r");
  if (fp == NULL) {
    printf("Errore nell'apertura del file di testo.\n");
    exit(1);
  }

  for (int i = 0; i < MAX_AUTOMOBILI; i++) {
    fscanf(fp, "%s %s %d %f %f %s", automobili[i].marca, automobili[i].modello,
           &automobili[i].cilindrata, &automobili[i].lunghezza,
           &automobili[i].peso, automobili[i].targa);
  }

  fclose(fp);
}

void salvaSuFileBinario(struct Automobile automobili[], const char *nomeFile) {
  FILE *fp = fopen(nomeFile, "wb");
  if (fp == NULL) {
    printf("Errore nell'apertura del file binario.\n");
    exit(1);
  }

  fwrite(automobili, sizeof(struct Automobile), MAX_AUTOMOBILI, fp);
  fclose(fp);
}

void estraiRecord(struct Automobile automobili[], const char *nomeFile,
                  int posizione) {
  FILE *fp = fopen(nomeFile, "rb");
  if (fp == NULL) {
    printf("Errore nell'apertura del file binario.\n");
    exit(1);
  }

  fseek(fp, posizione * sizeof(struct Automobile), SEEK_SET);
  struct Automobile record;
  fread(&record, sizeof(struct Automobile), 1, fp);

  printf("Record estratto dalla posizione %d:\n", posizione);
  printf("Marca: %s\n", record.marca);
  printf("Modello: %s\n", record.modello);
  printf("Cilindrata: %d\n", record.cilindrata);
  printf("Lunghezza: %.2f\n", record.lunghezza);
  printf("Peso: %.2f\n", record.peso);
  printf("Targa: %s\n", record.targa);

  fclose(fp);
}

void stampaArray(struct Automobile automobili[]) {
  printf("Dati delle automobili:\n");
  for (int i = 0; i < MAX_AUTOMOBILI; i++) {
    printf("Automobile %d:\n", i + 1);
    printf("Marca: %s\n", automobili[i].marca);
    printf("Modello: %s\n", automobili[i].modello);
    printf("Cilindrata: %d\n", automobili[i].cilindrata);
    printf("Lunghezza: %.2f\n", automobili[i].lunghezza);
    printf("Peso: %.2f\n", automobili[i].peso);
    printf("Targa: %s\n", automobili[i].targa);
    printf("\n");
  }
}

void inCoda(struct Automobile automobili[], const char *nomeFile,
            const char *nomeFileInd) {

  FILE *file = fopen(nomeFile, "a");
  if (file == NULL) {
    printf("Errore nell'apertura del file di testo.\n");
    exit(1);
  }

  FILE *fileInd = fopen(nomeFileInd, "a");
  if (fileInd == NULL) {
    printf("Errore nell'apertura del file di testo.\n");
    exit(1);
  }

  fseek(file, 0, SEEK_END);

  for (int i = 0; i < 1; i++) {
    char marca[MAX_STRING];
    char modello[MAX_STRING];
    int cilindrata;
    float lunghezza;
    float peso;
    char targa[MAX_STRING];

    randomize_car_data(marca, modello, &cilindrata, &lunghezza, &peso, targa);

    fprintf(file, "%s ", marca);
    fprintf(file, "%s ", modello);
    fprintf(file, "%d ", cilindrata);
    fprintf(file, "%.2f ", lunghezza);
    fprintf(file, "%.2f ", peso);
    fprintf(file, "%s\n", targa);

    fprintf(fileInd, "%s ", targa);
    fprintf(fileInd, "%d\n", i + 1);
  }
}

// Ricerca sequenziale su file testo, restituisce il tempo impiegato
double ricercaSeq(struct Automobile automobili[], const char *nomeFile,
                  const char *nomeFileInd, char targaRicerca[]) {
  clock_t start, end;
  start = clock();

  FILE *file = fopen(nomeFile, "r");
  if (file == NULL) {
    printf("Errore nell'apertura del file di testo.\n");
    exit(1);
  }

  FILE *fileInd = fopen(nomeFileInd, "r");
  if (fileInd == NULL) {
    printf("Errore nell'apertura del file di testo.\n");
    exit(1);
  }
  int flag = 0;
  for (int i = 0; i < MAX_AUTOMOBILI; i++) {
    fscanf(file, "%s %s %d %f %f %s", automobili[i].marca,
           automobili[i].modello, &automobili[i].cilindrata,
           &automobili[i].lunghezza, &automobili[i].peso, automobili[i].targa);

    if (strcmp(automobili[i].targa, targaRicerca) == 0) {
      // printf("%s = %s\n", automobili[i].targa, targaRicerca);
      printf("\n");
      printf("Risultato Trovato (sequenziale): ");
      printf("Automobile %d:\n", i + 1);
      printf("Marca: %s\n", automobili[i].marca);
      printf("Modello: %s\n", automobili[i].modello);
      printf("Cilindrata: %d\n", automobili[i].cilindrata);
      printf("Lunghezza: %.2f\n", automobili[i].lunghezza);
      printf("Peso: %.2f\n", automobili[i].peso);
      printf("Targa: %s\n", automobili[i].targa);
      printf("\n");
      flag = 1;
    }
  }
  if (flag == 0)
    printf("Risultato Non Trovato (sequenziale)\n");

  fclose(fileInd);
  fclose(file);

  end = clock();
  return ((double)(end - start)) / CLOCKS_PER_SEC;
}

// Ricerca binaria sull'indice ordinato, poi estrae il record dal file binario
// Restituisce il tempo impiegato
double ricercaBin(struct Voce voci[], int num_voci, const char *nomeFileBin,
                  char targaRicerca[]) {
  clock_t start, end;
  start = clock();

  // ricerca binaria per targa sull'array di voci gia' ordinato
  int sx = 0, dx = num_voci - 1, trovato = -1;
  while (sx <= dx) {
    int mid = (sx + dx) / 2;
    int cmp = strcmp(voci[mid].targa, targaRicerca);
    if (cmp == 0) {
      trovato = voci[mid].numero; // numero e' 1-based (posizione nel file)
      break;
    } else if (cmp < 0) {
      sx = mid + 1;
    } else {
      dx = mid - 1;
    }
  }

  if (trovato == -1) {
    printf("Risultato Non Trovato (binaria)\n");
  } else {
    FILE *fp = fopen(nomeFileBin, "rb");
    if (fp == NULL) {
      printf("Errore nell'apertura del file binario.\n");
      exit(1);
    }

    // salta direttamente al record tramite fseek (accesso diretto)
    fseek(fp, (trovato - 1) * sizeof(struct Automobile), SEEK_SET);
    struct Automobile record;
    fread(&record, sizeof(struct Automobile), 1, fp);
    fclose(fp);

    printf("\n");
    printf("Risultato Trovato (binaria): ");
    printf("Automobile %d:\n", trovato);
    printf("Marca: %s\n", record.marca);
    printf("Modello: %s\n", record.modello);
    printf("Cilindrata: %d\n", record.cilindrata);
    printf("Lunghezza: %.2f\n", record.lunghezza);
    printf("Peso: %.2f\n", record.peso);
    printf("Targa: %s\n", record.targa);
    printf("\n");
  }

  end = clock();
  return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
  struct Automobile automobili[MAX_AUTOMOBILI];
  struct Voce voce[MAX_AUTOMOBILI];
  const char *nomeFileTesto = "dati.txt";
  const char *nomeFileBinario = "dati.bin";
  const char *nomeFileIndice = "indice.txt";

  int db_caricato = 0; // diventa 1 dopo la generazione del database
  int scelta;

  printf("\n");
  printf("\n");
  printf("\n"); // leggibilita' da console di replit

  // // stampaArray(automobili); // stampa di tutto il file

  // loop principale del menu
  do {
    printf("=== Menu Gestione Automobili ===\n");
    printf("1. Genera database\n");
    printf("2. Cerca targa (sequenziale)\n");
    printf("3. Cerca targa (binaria)\n");
    printf("4. Confronta tempi\n");
    printf("5. Stampa record per posizione\n");
    printf("6. Esci\n");
    printf("Scelta: ");
    scanf("%d", &scelta);

    if (scelta == 1) {
      // genera i file e carica tutto in memoria
      write_car_data_to_file(nomeFileTesto, nomeFileIndice, MAX_AUTOMOBILI);
      leggiDaFileTesto(automobili, nomeFileTesto);
      leggiDaFileTestoIndice(voce, nomeFileIndice);
      salvaSuFileBinario(automobili, nomeFileBinario);
      db_caricato = 1;
      printf("Database generato con %d record.\n\n", MAX_AUTOMOBILI);

    } else if (scelta == 2) {
      if (!db_caricato) {
        printf("Generare prima il database (opzione 1)\n\n");
        continue;
      }
      char targa[MAX_STRING];
      printf("Inserisci targa: ");
      scanf("%s", targa);
      double tempo = ricercaSeq(automobili, nomeFileTesto, nomeFileIndice, targa);
      printf("Tempo sequenziale: %f s\n\n", tempo);

    } else if (scelta == 3) {
      if (!db_caricato) {
        printf("Generare prima il database (opzione 1)\n\n");
        continue;
      }
      char targa[MAX_STRING];
      printf("Inserisci targa: ");
      scanf("%s", targa);
      // ordina l'indice prima della ricerca binaria
      sort(voce, MAX_AUTOMOBILI);
      double tempo = ricercaBin(voce, MAX_AUTOMOBILI, nomeFileBinario, targa);
      printf("Tempo binario: %f s\n\n", tempo);

    } else if (scelta == 4) {
      if (!db_caricato) {
        printf("Generare prima il database (opzione 1)\n\n");
        continue;
      }
      char targa[MAX_STRING];
      printf("Inserisci targa: ");
      scanf("%s", targa);
      double tempoSeq = ricercaSeq(automobili, nomeFileTesto, nomeFileIndice, targa);
      sort(voce, MAX_AUTOMOBILI);
      double tempoBin = ricercaBin(voce, MAX_AUTOMOBILI, nomeFileBinario, targa);
      printf("--- Confronto Tempi di Ricerca ---\n");
      printf("Sequenziale: %f s\n", tempoSeq);
      printf("Binaria:     %f s\n", tempoBin);
      printf("----------------------------------\n\n");

    } else if (scelta == 5) {
      if (!db_caricato) {
        printf("Generare prima il database (opzione 1)\n\n");
        continue;
      }
      int posizione;
      printf("Inserisci posizione (0-%d): ", MAX_AUTOMOBILI - 1);
      scanf("%d", &posizione);
      estraiRecord(automobili, nomeFileBinario, posizione);
      printf("\n");

    } else if (scelta == 6) {
      printf("Uscita.\n");

    } else {
      printf("Opzione non valida.\n\n");
    }

  } while (scelta != 6);

  return 0;
}
