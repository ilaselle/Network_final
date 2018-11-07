#include "network.h"
#include "random.h"

void Network::resize(const size_t& n) {
	values.resize(n);
	RNG.normal(values);
}

bool Network::add_link(const size_t& a, const size_t& b) {
	if ((a >= values.size()) or  (b >= values.size()) or (a==b)) {
		return false;
	}
	for (auto node_linked : neighbors(a)) {
		if (node_linked == b) {
			return false;
		}
	}
	links.insert(std::pair<size_t, size_t>(a,b));
	links.insert(std::pair<size_t, size_t>(b,a));
	return true;
}

size_t Network::random_connect(const double& mean) { 
	links.clear();
	size_t size(values.size());
	std::vector<size_t> shuffle_values;
	for (size_t i(0); i < size; ++i) {
	shuffle_values.push_back(i);
	}
	
	std::vector<size_t> nb_degree(size);
	RNG.poisson(nb_degree, mean);
	
	for (size_t j(0); j < size; ++j) {
		
		RNG.shuffle(shuffle_values);
	
		if (nb_degree[j] >= size) {
			nb_degree[j] = size-1;
		}

		size_t x(0);
			while (degree(j) < nb_degree[j]) {
				size_t index(shuffle_values[x]);
				if (degree(index) >= nb_degree[index] or !add_link(j, index)) {
					size_t i(0);
					while((degree(i) >= nb_degree[i] or !add_link(j, i)) and i < size) {
						++i;
					} 
					if (i == size) {
						nb_degree[j] = degree(j);
					}
				}
				++x; 
			}
	}
	return links.size();  
}

size_t Network::set_values(const std::vector<double>& new_values) {
	size_t old_size(values.size());
	size_t new_size(new_values.size());
	
	if (new_size <= old_size) {
		for (size_t i(0); i < new_size; i++) {
			values[i] = new_values[i];
		}
		return new_size;
	} else {
		for (size_t i(0); i < old_size; i++) {
			values[i] = new_values[i];
		}
		return old_size;
	}
}

size_t Network::size() const {
	return values.size();
}

size_t Network::degree(const size_t &_n) const {
	return links.count(_n);
}

double Network::value(const size_t &_n) const {
	return values[_n];
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> tmp = values;
	std::sort (tmp.begin(), tmp.end());
	std::reverse (tmp.begin(), tmp.end());
	return tmp;
}

std::vector<size_t> Network::neighbors(const size_t& n) const {
	std::vector<size_t> neighbors;
	for (auto i : links) {
		if(i.first == n) {
			neighbors.push_back(i.second);
		}
	}
	return neighbors;
}
