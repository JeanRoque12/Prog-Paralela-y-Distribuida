class PaxosNode:
    def __init__(self, node_id):
        self.id = node_id
        self.promised_id = None
        self.accepted_id = None
        self.accepted_value = None
        self.alive = True

    def prepare(self, proposal_id):
        if not self.alive:
            return None
        if self.promised_id is None or proposal_id > self.promised_id:
            self.promised_id = proposal_id
            print(f"[Node {self.id}] Promete propuesta #{proposal_id}")
            return (self.accepted_id, self.accepted_value)
        else:
            print(f"[Node {self.id}] Rechaza propuesta #{proposal_id}")
            return None

    def accept(self, proposal_id, value):
        if not self.alive:
            return False
        if self.promised_id is None or proposal_id >= self.promised_id:
            self.promised_id = proposal_id
            self.accepted_id = proposal_id
            self.accepted_value = value
            print(f"[Node {self.id}] Acepta valor '{value}' en propuesta #{proposal_id}")
            return True
        return False

def simulate_paxos():
    nodes = [PaxosNode(i) for i in range(3)]
    proposal_id = 1
    value = "A=1"
    proposer = nodes[0]

    # Fase PREPARE
    responses = []
    print(f"[Proposer {proposer.id}] Inicia fase PREPARE con id #{proposal_id}")
    for node in nodes:
        response = node.prepare(proposal_id)
        if response is not None:
            responses.append(response)

    if len(responses) >= 2:
        # Fase ACCEPT
        print(f"\n[Proposer {proposer.id}] Suficientes promesas, inicia fase ACCEPT")
        accepted_count = 0
        for node in nodes:
            if node.accept(proposal_id, value):
                accepted_count += 1

        if accepted_count >= 2:
            print(f"\nCONSENSO alcanzado: valor '{value}' aceptado por mayoría")
        else:
            print("\nNo se alcanzó consenso")
    else:
        print("\nFase PREPARE fallida, no hay mayoría")

    # Fallo de un nodo
    print(f"\n[Simulación] Node 1 falla")
    nodes[1].alive = False

    # Nuevo intento
    proposal_id += 1
    print(f"\n[Proposer {proposer.id}] Reintenta propuesta #{proposal_id} con nodo fallando")
    responses = []
    for node in nodes:
        response = node.prepare(proposal_id)
        if response is not None:
            responses.append(response)

    if len(responses) >= 2:
        accepted_count = 0
        for node in nodes:
            if node.accept(proposal_id, value):
                accepted_count += 1
        if accepted_count >= 2:
            print(f"\nCONSENSO alcanzado tras fallo: '{value}'")
        else:
            print("\nNo se alcanzó consenso tras fallo")
    else:
        print("\n❌ No se pudo iniciar la fase ACCEPT tras fallo")

if __name__ == "__main__":
    simulate_paxos()
