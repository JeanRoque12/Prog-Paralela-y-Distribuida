import random

class Node:
    def __init__(self, node_id):
        self.id = node_id
        self.state = "follower"
        self.term = 0
        self.voted_for = None
        self.log = []
        self.alive = True

    def become_leader(self):
        self.state = "leader"
        print(f"[Node {self.id}] Se convierte en LÍDER (término {self.term})")

    def receive_vote_request(self, candidate_id, term):
        if not self.alive:
            return False
        if term > self.term and self.voted_for is None:
            self.voted_for = candidate_id
            self.term = term
            print(f"[Node {self.id}] Vota por Node {candidate_id}")
            return True
        return False

    def replicate_log(self, value):
        if self.state != "leader" or not self.alive:
            return
        print(f"[Node {self.id}] Replicando valor '{value}' a seguidores")
        for follower in cluster:
            if follower.id != self.id and follower.alive:
                follower.log.append(value)
                print(f"[Node {follower.id}] Recibió valor '{value}'")

def simulate_raft():
    global cluster
    cluster = [Node(i) for i in range(3)]

    # Primera elección
    candidate = cluster[0]
    candidate.term += 1
    candidate_votes = 1
    print(f"[Node {candidate.id}] Solicita votos para término {candidate.term}")
    for node in cluster[1:]:
        if node.receive_vote_request(candidate.id, candidate.term):
            candidate_votes += 1

    if candidate_votes >= 2:
        candidate.become_leader()
        candidate.replicate_log("A=1")

    # Simular fallo del líder
    print(f"\n[Simulación] Node {candidate.id} falla")
    candidate.alive = False

    # Nueva elección
    new_candidate = cluster[1]
    new_candidate.term += 1
    votes = 1
    print(f"[Node {new_candidate.id}] Solicita votos tras fallo del líder")
    for node in cluster:
        if node.id != new_candidate.id and node.receive_vote_request(new_candidate.id, new_candidate.term):
            votes += 1

    if votes >= 2:
        new_candidate.become_leader()
        new_candidate.replicate_log("A=1")

if __name__ == "__main__":
    simulate_raft()
