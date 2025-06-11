# Simulador de Memoria Paginada

Este projeto implementa um simulador de memoria virtual paginada com três tipos diferentes de tabelas de paginas e politicas de substituicao.

- **Modos de operacao**:
  - ⌨️ Interativo: usuario digita enderecos manualmente
  - 🎲 Aleatorio: gera automaticamente 10 enderecos

- **Relatorios detalhados** apos cada operacao
- 📊 Visualizacao do estado atual da memoria e estatisticas

## 🔧 Compilacao

```bash
mingw32-make all
```

##  Execucao

```bash
# Tabela de 1 nivel no modo interativo
./simulador 1 i

# Tabela de 2 niveis no modo aleatorio
./simulador 2 r

# Tabela invertida (modo interativo padrao)
./simulador inv
```