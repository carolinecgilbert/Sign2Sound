# Get inputted letter's WAV data
import sys
 
# adding wav_helpers to the system path
sys.path.insert(0, 'wav_helpers')

# Import all helper functions for each letter
from a_helper import get_a_data
from b_helper import get_b_data
from c_helper import get_c_data
from d_helper import get_d_data
from e_helper import get_e_data
from f_helper import get_f_data
from g_helper import get_g_data
from h_helper import get_h_data
from i_helper import get_i_data
from k_helper import get_k_data
from l_helper import get_l_data
from m_helper import get_m_data
from n_helper import get_n_data
from o_helper import get_o_data
from p_helper import get_p_data
from q_helper import get_q_data
from r_helper import get_r_data
from s_helper import get_s_data
from t_helper import get_t_data
from u_helper import get_u_data
from v_helper import get_v_data
from w_helper import get_w_data
from x_helper import get_x_data
from y_helper import get_y_data

# Return WAV data for inputted letter
def get_data(letter):
    # Retrieve a.wav data
    if letter == 'a' or letter == 'A':
        return get_a_data()
    # Retrieve b.wav data
    elif letter == 'b' or letter == 'B':
        return get_b_data()
    # Retrieve c.wav data
    elif letter == 'c' or letter == 'C':
        return get_c_data()
    # Retrieve d.wav data
    elif letter == 'd' or letter == 'D':
        return get_d_data()
    # Retrieve e.wav data
    elif letter == 'e' or letter == 'E':
        return get_e_data()
    # Retrieve f.wav data
    elif letter == 'f' or letter == 'F':
        return get_f_data()
    # Retrieve g.wav data
    elif letter == 'g' or letter == 'G':
        return get_g_data()
    # Retrieve h.wav data
    elif letter == 'h' or letter == 'H':
        return get_h_data()
    # Retrieve i.wav data
    elif letter == 'i' or letter == 'I':
        return get_i_data()

    # NO J.wav data

    # Retrieve k.wav data
    elif letter == 'k' or letter == 'K':
        return get_k_data()
    # Retrieve l.wav data
    elif letter == 'l' or letter == 'L':
        return get_l_data()
    # Retrieve m.wav data
    elif letter == 'm' or letter == 'M':
        return get_m_data()
    # Retrieve n.wav data
    elif letter == 'n' or letter == 'N':
        return get_n_data()
    # Retrieve o.wav data
    elif letter == 'o' or letter == 'O':
        return get_o_data()
    # Retrieve p.wav data
    elif letter == 'p' or letter == 'P':
        return get_p_data()
    # Retrieve q.wav data
    elif letter == 'q' or letter == 'Q':
        return get_q_data()
    # Retrieve r.wav data
    elif letter == 'r' or letter == 'R':
        return get_r_data()
    # Retrieve s.wav data
    elif letter == 's' or letter == 'S':
        return get_s_data()
    # Retrieve t.wav data
    elif letter == 't' or letter == 'T':
        return get_t_data()
    # Retrieve u.wav data
    elif letter == 'u' or letter == 'U':
        return get_u_data()
    # Retrieve v.wav data
    elif letter == 'v' or letter == 'V':
        return get_v_data()
    # Retrieve w.wav data
    elif letter == 'w' or letter == 'W':
        return get_w_data()
    # Retrieve x.wav data
    elif letter == 'x' or letter == 'X':
        return get_x_data()
    # Retrieve y.wav data
    elif letter == 'y' or letter == 'Y':
        return get_y_data()
    
    # NO Z.wav data

    