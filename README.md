# Lalapkg

## DESC

O Gerenciador de Pacotes lalapkg é um gerenciador de pacotes que evoluiu de um simples script Bash para uma robusta aplicação C++. Desenvolvido para lidar com instalação de pacotes focado no ambiente LFS, o lalapkg oferece uma maneira eficiente de gerenciar pacotes em ambientes Unix-like.

## OBS

Este pequeno projeto foi desenvolvido por pura diversão e como uma forma de praticar minha lógica de programação, visando aprimorar gradualmente minhas habilidades na linguagem C++.

## Recursos
- **Instalação de Pacotes**: Gerencia o download, desempacotamento e instalação de pacotes de software.
- **Criação de Pacotes**: Suporta a construção e empacotamento de software a partir do código fonte.
- **Informações sobre Pacotes**: Exibe informações detalhadas sobre pacotes.
- **Indicadores de Progresso**: Fornece feedback em tempo real durante operações prolongadas com indicadores de progresso animados.
- **Configuração**: Utiliza um arquivo de configuração para gerenciar definições e caminhos.

## Foco no Linux From Scratch (LFS)

Este projeto foi desenvolvido com um foco específico em ambientes **Linux From Scratch (LFS)**. O LFS é um projeto que fornece instruções detalhadas sobre como construir um sistema Linux a partir do zero, a partir dos códigos-fonte. Isso proporciona um entendimento profundo e controle sobre cada componente do sistema.

O objetivo é fornecer uma ferramenta prática e eficaz para gerenciar pacotes em um ambiente LFS, alinhando-se com a filosofia de simplicidade e controle do projeto.

## Status do Projeto

O Gerenciador de Pacotes lalapkg é um projeto em andamento. Embora já forneça funcionalidades básicas para instalação, criação e visualização de informações de pacotes, ainda há várias melhorias e recursos planejados para futuras versões.

### Funcionalidades Planejadas

- **Desinstalação de Pacotes (`-u`)**: Implementar a função `unmerge` para desinstalar pacotes do sistema. Isso permitirá remover pacotes instalados anteriormente, limpando arquivos e dependências associadas.

- **Resolução de Dependências**: Adicionar suporte para resolução automática de dependências durante a instalação de pacotes. Isso garantirá que todas as dependências necessárias sejam instaladas e gerenciadas corretamente.

- **Suporte para Parâmetros Adicionais**: Expandir a interface de linha de comando (CLI) para incluir suporte para parâmetros adicionais. Isso permitirá que os usuários especifiquem opções adicionais e flags para personalizar o comportamento do CPM, como a definição de ambientes específicos, opções de configuração avançadas e parâmetros para ajustar o processo de instalação e criação de pacotes.

-  **Documentação e Exemplos**: Expandir a documentação com exemplos de uso mais detalhados e instruções adicionais para facilitar a integração e o uso do lalapkg.

### Pré-requisitos

- Compilador C++ (por exemplo, `g++`)
- Biblioteca `libconfig` para gerenciamento de configuração

### Compilação

Como o projeto ainda está no começo, a compilação é feita de um jeito manual:

1. **Clone o Repositório**:

   ```sh
    git clone https://github.com/Linuxperoxo/lalapkg-cpp.git
    cd lalapkg-cpp
2. **Compile o Projeto**

    ```sh
    make clean
    make

Agora foi pode executar o bin que está em `bin/lalapkg`

### Configuração

Antes de usar o lalapkg, verifique se o arquivo de configuração `/etc/lala.conf` está configurado corretamente. Este arquivo define os caminhos e configurações utilizados pelo lalapkg.

### Comandos

O Gerenciador suporta os seguintes parâmetros:

- **Instalar um Pacote**:
  ```sh
  lalapkg -e <nome_do_pacote>

- **Ver informações de um pacote**:
  ```sh
  lalapkg -i<tipo_de_info> <nome_do_pacote>

- a: Todas as informações
- v: Versão do pacote
- s: URL do fonte do pacote


- **Atualizar Repositório**:

O repositório ainda não está aberto ao público, porém você pode criar seus script. Leia o arquivo que está em `doc/repo.doc` para obter mais informações de como criar seus scripts.
  
## Contribuindo

- Contribuições para o lalapkg são bem-vindas! Por favor, envie problemas ou solicitações de pull via o repositório no GitHub. Certifique-se de que seu código segue o estilo existente e inclui testes apropriados.

## Autor

- Linuxperoxo: https://github.com/Linuxperoxo
