# Mapa Imperium para MuOnline
### Sistema do mapa imperium
Um mapa desenvolvido totalmente do zero para a versão 97d do Muonline.
As quests customizadas fazem do mapa um estilo de calabouço onde o player irá para farmar itens raros no jogo, além do sistema de ranking do mapa para aumentar ainda mais a competitividade do jogo.

### Parte gráfica e NPC
A parte gráfica do mapa já foi finalizada, também os NPCs que irão interagir com
o player já estão terminados e testados.

### Salas
O mapa conta com 10 salas, como se fosse um castelo onde o player vai avançando a medida que cumpre as missões no mapa. A última sala é apenas um local de premiação.

#### Configurando os Boss do mapa
Os boss são monstros normais que devem ser adicionados ao MonsterSetBase.txt, a diferença do boss da sala 5 é que sua força e tempo de respawn são editados na configuração do mapa.
##### O único boss que nasce automaticamente é o da sala 9 que é conforme o número de monstros mortos por algum player na sala.
Os drops dos Boss do mapa, que estão na sala 5 (indefinido), sala 8 (SoulTree C), sala 9 (BugBear K)
ou qualquer outro boss que quiserem adicionar, são configurados no arquivo DropSystem.txt
basta colocar o id do monstro para o item inserido na lista de drop e a porcentagem que deseja que o item drope.

### Premiação do mapa
A maioria da premiação são drops de item exclusivos que podem ser configurados em cada sala(DropSystem.txt). Na sala 4 tem uma premiação fixa configurável por concluir a missão.
#### Na premiação final existe um ranking salvo (que pode ser usado como quiser) e também um item Teleport +12 que garante 1 move direto para a metade do mapa (Sala 5). Para isso basta chegar até a sala 10.

## Quests em cada sala
###### Sala 1 e 2
Quest inicia quando o player matar o primeiro mob. Terá que matar xx quantia de mobs na 1 para ir para a 2 e xx quantia na 2 para ir para a 3.
###### Sala 3
É preciso ter Creation para passar pelo cristal, e na sala tem que matar xx quantia de mobs para ir para a sala 4
###### Sala 4
Player vai matar xx quantia de mobs até o Broken Device aparecer por xx segundos em algum lugar da sala, o player precisa achá-lo e clicar nele antes que suma de novo para ir para próxima sala.
###### Sala 5
Na sala 5 o boss irá aparecer conforme configurado pelo adm, o player terá que matar ele para passar pelos guardas na ponte e ir para a sala 6.
###### Sala 6
É uma sala sem quests onde pode ser adicionado boss ou spots livres. O portal para a sala 7 abre de xx em xx tempo (configurado pelo adm) e ficar aberta por algum tempo.
###### Sala 7
É uma sala onde tem que descobrir a chave secreta matando as figuras uma a uma. Após completar a chave e aparecer "Boa sorte..." clicar no npc e ver se passa.
Se erra vai voltar lá para o início da sala 5.
###### Sala 8
Tem que matar os mobs A, B e C em sequência e ao matar o C, terá 1 minuto para clicar no portal para sala 9.
###### Sala 9
Terá que matar xx quantia de mobs para aparecer o último Boss do mapa, ao matá-lo o portal será liberado para ir a sala 10.
###### Sala 10
É uma sala apenas de premiação: ao chegar lá terá um Teleport pra cidade +12 que pode ser usado para ir direto para a sala 5 do mapa, também tem uma contagem de ranking para quem chegar até lá.
Opcional colocar spots, boss etc.
