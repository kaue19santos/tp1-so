# TP1 SO - Syscall getcnt

## Objetivo

Modificar o xv6 para:

- Contar quantas vezes cada syscall foi chamada desde o boot.
- Expor a syscall `getcnt(int syscall_number)` para consultar esse contador.
- Disponibilizar o programa de usuario `getcnt` para chamar essa syscall.

## Estrutura de dados escolhida

A contagem foi implementada com um vetor global em `kernel/syscall.c`:

- `static uint64 syscall_count[NELEM(syscalls)];`

Motivo da escolha:

- Acesso O(1) por indice da syscall.
- Simplicidade de integracao com o dispatcher da funcao `syscall()`.
- Nao depende de estado por processo, refletindo total desde o boot.

## Onde a contagem acontece

No dispatcher em `kernel/syscall.c`, para cada syscall valida:

- `syscall_count[num]++;`

Essa linha e executada antes da chamada do handler real da syscall.

## Implementacao da syscall getcnt

Fluxo da implementacao:

1. Numero da syscall definido em `kernel/syscall.h` como `SYS_getcnt`.
2. Handler `sys_getcnt()` implementado em `kernel/sysproc.c`.
3. Registro no vetor `syscalls[]` em `kernel/syscall.c`.
4. Funcao auxiliar `get_syscall_count(int)` para validar indice e retornar contagem.
5. Integracao no user-space com:
   - prototipo em `user/user.h`
   - stub em `user/usys.pl`

## Programa de usuario getcnt

Arquivo: `user/getcnt.c`

Comportamento:

- Recebe um argumento inteiro (`syscall_number`).
- Chama `getcnt(syscall_number)`.
- Exibe a contagem no terminal.
- Trata erro para entrada invalida e numero de syscall invalido.

Exemplos esperados:

```sh
$ getcnt 22
syscall 22 has been called 3 times

$ getcnt 999
getcnt: invalid syscall number 999
```

## Build e execucao

### Opcao A (host)

```sh
make clean
make -j4
make qemu
```

Observacao: o Makefile exige QEMU >= 7.2.

### Opcao B (container, recomendado quando o host nao atende a versao)

```sh
./container/run-xv6-container.sh
```

## Testes executados (roteiro)

No shell do xv6:

```sh
getcnt 22
getcnt 22
getcnt 16
echo oi
getcnt 16
getcnt 999
```

Criterios de aprovacao:

- O segundo `getcnt 22` deve ser maior que o primeiro.
- Depois de `echo oi`, o valor de `getcnt 16` (write) deve aumentar.
- `getcnt 999` deve retornar erro (numero invalido).

## Diffs para mostrar no PDF

Gerar os quatro blocos obrigatorios com:

```sh
git diff -- kernel/syscall.c
git diff -- kernel/sysproc.c
git diff -- kernel/syscall.h user/usys.pl user/user.h
git diff -- user/getcnt.c Makefile
```

Sugestao de discussao no PDF:

1. Estrutura de dados da contagem (`syscall_count[]`).
2. Ponto de incremento no dispatcher (`syscall()`).
3. Implementacao da syscall `getcnt` no kernel.
4. Integracao user-space (stub/prototipo/programa `getcnt` e `UPROGS`).

## Limites conhecidos

- Contagem e global desde o boot (nao por processo).
- Syscalls invalidas nao sao contabilizadas no vetor de validas.
- A syscall `getcnt` tambem e contabilizada como qualquer outra syscall valida.
