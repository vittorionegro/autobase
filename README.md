# autobase

Gestione di un database di 10.000 automobili in C, con ricerca sequenziale e binaria su indice ordinato.

Progetto scolastico — C99, singolo file.

## Funzionalità

| Opzione | Descrizione |
|---------|-------------|
| 1 | Genera 10.000 auto casuali, scrive `dati.txt`, `indice.txt`, `dati.bin` |
| 2 | Ricerca per targa — sequenziale sul file testo, con timing |
| 3 | Ricerca per targa — binaria sull'indice ordinato + accesso diretto al binario, con timing |
| 4 | Esegue entrambe le ricerche e confronta i tempi fianco a fianco |
| 5 | Estrae e stampa un record per posizione dal file binario |
| 6 | Esci |

## Struttura

```
main.c          — tutto il codice
dati.txt        — generato a runtime (10.000 record testuali)
indice.txt      — generato a runtime (targa + numero record)
dati.bin        — generato a runtime (record in formato binario)
```

## Download

Scarica `autobase.exe` dalla pagina [Releases](../../releases) ed eseguilo direttamente — nessuna installazione richiesta.

**Compatibilità:**
- Windows 10 / 11: funziona senza dipendenze aggiuntive
- Windows 7 / 8.1: richiede il [Visual C++ Redistributable 2015–2022](https://aka.ms/vs/17/release/vc_redist.x64.exe)

## Requisiti (solo per compilare)

- GCC (MinGW-w64 su Windows)
- VS Code con estensione C/C++ (Microsoft)

## Come compilare ed eseguire

**VS Code:** premi `F5` — compila e avvia automaticamente.

**Da terminale:**
```bash
gcc -O2 -std=c99 -o autobase.exe main.c
./autobase.exe
```

## Installazione GCC su Windows

```powershell
winget install BrechtSanders.WinLibs.POSIX.UCRT
```

Riavvia il terminale dopo l'installazione.
