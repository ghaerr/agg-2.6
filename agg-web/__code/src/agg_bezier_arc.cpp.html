<HTML><HEAD><TITLE>Anti-Grain Geometry - </TITLE>
<META http-equiv="Content-Type" content="text/html"/>
<LINK rel="stylesheet" type="text/css" href="../../agg.styles.css"/>
</HEAD>
<TABLE width="640px" border="0" cellspacing="0" cellpadding="0"><TR><TD><PRE><SPAN class="rem">//----------------------------------------------------------------------------</SPAN>
<SPAN class="rem">// Anti-Grain Geometry (AGG) - Version 2.5</SPAN>
<SPAN class="rem">// A high quality rendering engine for C++</SPAN>
<SPAN class="rem">// Copyright (C) 2002-2006 Maxim Shemanarev</SPAN>
<SPAN class="rem">// Contact: mcseem@antigrain.com</SPAN>
<SPAN class="rem">//          mcseemagg@yahoo.com</SPAN>
<SPAN class="rem">//          http://antigrain.com</SPAN>
<SPAN class="rem">// </SPAN>
<SPAN class="rem">// AGG is free software; you can redistribute it and/or</SPAN>
<SPAN class="rem">// modify it under the terms of the GNU General Public License</SPAN>
<SPAN class="rem">// as published by the Free Software Foundation; either version 2</SPAN>
<SPAN class="rem">// of the License, or (at your option) any later version.</SPAN>
<SPAN class="rem">// </SPAN>
<SPAN class="rem">// AGG is distributed in the hope that it will be useful,</SPAN>
<SPAN class="rem">// but WITHOUT ANY WARRANTY; without even the implied warranty of</SPAN>
<SPAN class="rem">// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the</SPAN>
<SPAN class="rem">// GNU General Public License for more details.</SPAN>
<SPAN class="rem">// </SPAN>
<SPAN class="rem">// You should have received a copy of the GNU General Public License</SPAN>
<SPAN class="rem">// along with AGG; if not, write to the Free Software</SPAN>
<SPAN class="rem">// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, </SPAN>
<SPAN class="rem">// MA 02110-1301, USA.</SPAN>
<SPAN class="rem">//----------------------------------------------------------------------------</SPAN>

<SPAN class="kw2">#include</SPAN> <SPAN class="op">&lt;</SPAN>math<SPAN class="op">.</SPAN>h<SPAN class="op">&gt;</SPAN>
<SPAN class="kw2">#include</SPAN> <SPAN class="str">&quot;<A href="../include/agg_bezier_arc.h.html">agg_bezier_arc.h</A>&quot;</SPAN>


<SPAN class="kw1">namespace</SPAN> agg
<SPAN class="op">{</SPAN>

    <SPAN class="rem">// This epsilon is used to prevent us from adding degenerate curves </SPAN>
    <SPAN class="rem">// (converging to a single point).</SPAN>
    <SPAN class="rem">// The value isn&#039;t very critical. Function <A href="agg_bezier_arc.cpp.html#arc_to_bezier">arc_to_bezier</A>() has a limit </SPAN>
    <SPAN class="rem">// of the sweep_angle. If fabs(sweep_angle) exceeds <A href="../include/agg_basics.h.html#pi">pi</A>/2 the curve </SPAN>
    <SPAN class="rem">// becomes inaccurate. But slight exceeding is quite appropriate.</SPAN>
    <SPAN class="rem">//-------------------------------------------------<A name="bezier_arc_angle_epsilon"><B></B></A><A href="agg_bezier_arc.cpp.html#bezier_arc_angle_epsilon">bezier_arc_angle_epsilon</A></SPAN>
    <SPAN class="kw1">const</SPAN> <SPAN class="kw1">double</SPAN> <A href="agg_bezier_arc.cpp.html#bezier_arc_angle_epsilon">bezier_arc_angle_epsilon</A> <SPAN class="op">=</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">01</SPAN><SPAN class="op">;</SPAN>

    <SPAN class="rem">//------------------------------------------------------------<A name="arc_to_bezier"><B></B></A><A href="agg_bezier_arc.cpp.html#arc_to_bezier">arc_to_bezier</A></SPAN>
    <SPAN class="kw1">void</SPAN> <A href="agg_bezier_arc.cpp.html#arc_to_bezier">arc_to_bezier</A><SPAN class="op">(</SPAN><SPAN class="kw1">double</SPAN> cx<SPAN class="op">,</SPAN> <SPAN class="kw1">double</SPAN> cy<SPAN class="op">,</SPAN> <SPAN class="kw1">double</SPAN> rx<SPAN class="op">,</SPAN> <SPAN class="kw1">double</SPAN> ry<SPAN class="op">,</SPAN> 
                       <SPAN class="kw1">double</SPAN> start_angle<SPAN class="op">,</SPAN> <SPAN class="kw1">double</SPAN> sweep_angle<SPAN class="op">,</SPAN>
                       <SPAN class="kw1">double</SPAN><SPAN class="op">*</SPAN> curve<SPAN class="op">)</SPAN>
    <SPAN class="op">{</SPAN>
        <SPAN class="kw1">double</SPAN> x0 <SPAN class="op">=</SPAN> cos<SPAN class="op">(</SPAN>sweep_angle <SPAN class="op">/</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> y0 <SPAN class="op">=</SPAN> sin<SPAN class="op">(</SPAN>sweep_angle <SPAN class="op">/</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> tx <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN><SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN> <SPAN class="op">-</SPAN> x0<SPAN class="op">)</SPAN> <SPAN class="op">*</SPAN> <SPAN class="num">4</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN> <SPAN class="op">/</SPAN> <SPAN class="num">3</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> ty <SPAN class="op">=</SPAN> y0 <SPAN class="op">-</SPAN> tx <SPAN class="op">*</SPAN> x0 <SPAN class="op">/</SPAN> y0<SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> px<SPAN class="op">[</SPAN><SPAN class="num">4</SPAN><SPAN class="op">]</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> py<SPAN class="op">[</SPAN><SPAN class="num">4</SPAN><SPAN class="op">]</SPAN><SPAN class="op">;</SPAN>
        px<SPAN class="op">[</SPAN><SPAN class="num">0</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN>  x0<SPAN class="op">;</SPAN>
        py<SPAN class="op">[</SPAN><SPAN class="num">0</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> <SPAN class="op">-</SPAN>y0<SPAN class="op">;</SPAN>
        px<SPAN class="op">[</SPAN><SPAN class="num">1</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN>  x0 <SPAN class="op">+</SPAN> tx<SPAN class="op">;</SPAN>
        py<SPAN class="op">[</SPAN><SPAN class="num">1</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> <SPAN class="op">-</SPAN>ty<SPAN class="op">;</SPAN>
        px<SPAN class="op">[</SPAN><SPAN class="num">2</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN>  x0 <SPAN class="op">+</SPAN> tx<SPAN class="op">;</SPAN>
        py<SPAN class="op">[</SPAN><SPAN class="num">2</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN>  ty<SPAN class="op">;</SPAN>
        px<SPAN class="op">[</SPAN><SPAN class="num">3</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN>  x0<SPAN class="op">;</SPAN>
        py<SPAN class="op">[</SPAN><SPAN class="num">3</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN>  y0<SPAN class="op">;</SPAN>

        <SPAN class="kw1">double</SPAN> sn <SPAN class="op">=</SPAN> sin<SPAN class="op">(</SPAN>start_angle <SPAN class="op">+</SPAN> sweep_angle <SPAN class="op">/</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> cs <SPAN class="op">=</SPAN> cos<SPAN class="op">(</SPAN>start_angle <SPAN class="op">+</SPAN> sweep_angle <SPAN class="op">/</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>

        <SPAN class="kw1">unsigned</SPAN> i<SPAN class="op">;</SPAN>
        <SPAN class="kw1">for</SPAN><SPAN class="op">(</SPAN>i <SPAN class="op">=</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">;</SPAN> i <SPAN class="op">&lt;</SPAN> <SPAN class="num">4</SPAN><SPAN class="op">;</SPAN> i<SPAN class="op">++</SPAN><SPAN class="op">)</SPAN>
        <SPAN class="op">{</SPAN>
            curve<SPAN class="op">[</SPAN>i <SPAN class="op">*</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">]</SPAN>     <SPAN class="op">=</SPAN> cx <SPAN class="op">+</SPAN> rx <SPAN class="op">*</SPAN> <SPAN class="op">(</SPAN>px<SPAN class="op">[</SPAN>i<SPAN class="op">]</SPAN> <SPAN class="op">*</SPAN> cs <SPAN class="op">-</SPAN> py<SPAN class="op">[</SPAN>i<SPAN class="op">]</SPAN> <SPAN class="op">*</SPAN> sn<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
            curve<SPAN class="op">[</SPAN>i <SPAN class="op">*</SPAN> <SPAN class="num">2</SPAN> <SPAN class="op">+</SPAN> <SPAN class="num">1</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> cy <SPAN class="op">+</SPAN> ry <SPAN class="op">*</SPAN> <SPAN class="op">(</SPAN>px<SPAN class="op">[</SPAN>i<SPAN class="op">]</SPAN> <SPAN class="op">*</SPAN> sn <SPAN class="op">+</SPAN> py<SPAN class="op">[</SPAN>i<SPAN class="op">]</SPAN> <SPAN class="op">*</SPAN> cs<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="op">}</SPAN>
    <SPAN class="op">}</SPAN>



    <SPAN class="rem">//------------------------------------------------------------------------</SPAN>
    <SPAN class="kw1">void</SPAN> bezier_arc::init<SPAN class="op">(</SPAN><SPAN class="kw1">double</SPAN> x<SPAN class="op">,</SPAN>  <SPAN class="kw1">double</SPAN> y<SPAN class="op">,</SPAN> 
                          <SPAN class="kw1">double</SPAN> rx<SPAN class="op">,</SPAN> <SPAN class="kw1">double</SPAN> ry<SPAN class="op">,</SPAN> 
                          <SPAN class="kw1">double</SPAN> start_angle<SPAN class="op">,</SPAN> 
                          <SPAN class="kw1">double</SPAN> sweep_angle<SPAN class="op">)</SPAN>
    <SPAN class="op">{</SPAN>
        start_angle <SPAN class="op">=</SPAN> fmod<SPAN class="op">(</SPAN>start_angle<SPAN class="op">,</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN> <SPAN class="op">*</SPAN> <A href="../include/agg_basics.h.html#pi">pi</A><SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>sweep_angle <SPAN class="op">&gt;=</SPAN>  <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN> <SPAN class="op">*</SPAN> <A href="../include/agg_basics.h.html#pi">pi</A><SPAN class="op">)</SPAN> sweep_angle <SPAN class="op">=</SPAN>  <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN> <SPAN class="op">*</SPAN> <A href="../include/agg_basics.h.html#pi">pi</A><SPAN class="op">;</SPAN>
        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>sweep_angle <SPAN class="op">&lt;=</SPAN> <SPAN class="op">-</SPAN><SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN> <SPAN class="op">*</SPAN> <A href="../include/agg_basics.h.html#pi">pi</A><SPAN class="op">)</SPAN> sweep_angle <SPAN class="op">=</SPAN> <SPAN class="op">-</SPAN><SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN> <SPAN class="op">*</SPAN> <A href="../include/agg_basics.h.html#pi">pi</A><SPAN class="op">;</SPAN>

        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>fabs<SPAN class="op">(</SPAN>sweep_angle<SPAN class="op">)</SPAN> <SPAN class="op">&lt;</SPAN> <SPAN class="num">1e</SPAN><SPAN class="op">-</SPAN><SPAN class="num">10</SPAN><SPAN class="op">)</SPAN>
        <SPAN class="op">{</SPAN>
            m_num_vertices <SPAN class="op">=</SPAN> <SPAN class="num">4</SPAN><SPAN class="op">;</SPAN>
            m_cmd <SPAN class="op">=</SPAN> <A href="../include/agg_basics.h.html#path_cmd_line_to">path_cmd_line_to</A><SPAN class="op">;</SPAN>
            m_vertices<SPAN class="op">[</SPAN><SPAN class="num">0</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> x <SPAN class="op">+</SPAN> rx <SPAN class="op">*</SPAN> cos<SPAN class="op">(</SPAN>start_angle<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
            m_vertices<SPAN class="op">[</SPAN><SPAN class="num">1</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> y <SPAN class="op">+</SPAN> ry <SPAN class="op">*</SPAN> sin<SPAN class="op">(</SPAN>start_angle<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
            m_vertices<SPAN class="op">[</SPAN><SPAN class="num">2</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> x <SPAN class="op">+</SPAN> rx <SPAN class="op">*</SPAN> cos<SPAN class="op">(</SPAN>start_angle <SPAN class="op">+</SPAN> sweep_angle<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
            m_vertices<SPAN class="op">[</SPAN><SPAN class="num">3</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> y <SPAN class="op">+</SPAN> ry <SPAN class="op">*</SPAN> sin<SPAN class="op">(</SPAN>start_angle <SPAN class="op">+</SPAN> sweep_angle<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
            <SPAN class="kw1">return</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="op">}</SPAN>

        <SPAN class="kw1">double</SPAN> total_sweep <SPAN class="op">=</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> local_sweep <SPAN class="op">=</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> prev_sweep<SPAN class="op">;</SPAN>
        m_num_vertices <SPAN class="op">=</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">;</SPAN>
        m_cmd <SPAN class="op">=</SPAN> <A href="../include/agg_basics.h.html#path_cmd_curve4">path_cmd_curve4</A><SPAN class="op">;</SPAN>
        <SPAN class="kw1">bool</SPAN> done <SPAN class="op">=</SPAN> <SPAN class="kw1">false</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">do</SPAN>
        <SPAN class="op">{</SPAN>
            <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>sweep_angle <SPAN class="op">&lt;</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN>
            <SPAN class="op">{</SPAN>
                prev_sweep  <SPAN class="op">=</SPAN> total_sweep<SPAN class="op">;</SPAN>
                local_sweep <SPAN class="op">=</SPAN> <SPAN class="op">-</SPAN><A href="../include/agg_basics.h.html#pi">pi</A> <SPAN class="op">*</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">5</SPAN><SPAN class="op">;</SPAN>
                total_sweep <SPAN class="op">-=</SPAN> <A href="../include/agg_basics.h.html#pi">pi</A> <SPAN class="op">*</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">5</SPAN><SPAN class="op">;</SPAN>
                <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>total_sweep <SPAN class="op">&lt;=</SPAN> sweep_angle <SPAN class="op">+</SPAN> <A href="agg_bezier_arc.cpp.html#bezier_arc_angle_epsilon">bezier_arc_angle_epsilon</A><SPAN class="op">)</SPAN>
                <SPAN class="op">{</SPAN>
                    local_sweep <SPAN class="op">=</SPAN> sweep_angle <SPAN class="op">-</SPAN> prev_sweep<SPAN class="op">;</SPAN>
                    done <SPAN class="op">=</SPAN> <SPAN class="kw1">true</SPAN><SPAN class="op">;</SPAN>
                <SPAN class="op">}</SPAN>
            <SPAN class="op">}</SPAN>
            <SPAN class="kw1">else</SPAN>
            <SPAN class="op">{</SPAN>
                prev_sweep  <SPAN class="op">=</SPAN> total_sweep<SPAN class="op">;</SPAN>
                local_sweep <SPAN class="op">=</SPAN>  <A href="../include/agg_basics.h.html#pi">pi</A> <SPAN class="op">*</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">5</SPAN><SPAN class="op">;</SPAN>
                total_sweep <SPAN class="op">+=</SPAN> <A href="../include/agg_basics.h.html#pi">pi</A> <SPAN class="op">*</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">5</SPAN><SPAN class="op">;</SPAN>
                <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>total_sweep <SPAN class="op">&gt;=</SPAN> sweep_angle <SPAN class="op">-</SPAN> <A href="agg_bezier_arc.cpp.html#bezier_arc_angle_epsilon">bezier_arc_angle_epsilon</A><SPAN class="op">)</SPAN>
                <SPAN class="op">{</SPAN>
                    local_sweep <SPAN class="op">=</SPAN> sweep_angle <SPAN class="op">-</SPAN> prev_sweep<SPAN class="op">;</SPAN>
                    done <SPAN class="op">=</SPAN> <SPAN class="kw1">true</SPAN><SPAN class="op">;</SPAN>
                <SPAN class="op">}</SPAN>
            <SPAN class="op">}</SPAN>

            <A href="agg_bezier_arc.cpp.html#arc_to_bezier">arc_to_bezier</A><SPAN class="op">(</SPAN>x<SPAN class="op">,</SPAN> y<SPAN class="op">,</SPAN> rx<SPAN class="op">,</SPAN> ry<SPAN class="op">,</SPAN> 
                          start_angle<SPAN class="op">,</SPAN> 
                          local_sweep<SPAN class="op">,</SPAN> 
                          m_vertices <SPAN class="op">+</SPAN> m_num_vertices <SPAN class="op">-</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>

            m_num_vertices <SPAN class="op">+=</SPAN> <SPAN class="num">6</SPAN><SPAN class="op">;</SPAN>
            start_angle <SPAN class="op">+=</SPAN> local_sweep<SPAN class="op">;</SPAN>
        <SPAN class="op">}</SPAN>
        <SPAN class="kw1">while</SPAN><SPAN class="op">(</SPAN><SPAN class="op">!</SPAN>done <SPAN class="op">&amp;&amp;</SPAN> m_num_vertices <SPAN class="op">&lt;</SPAN> <SPAN class="num">26</SPAN><SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
    <SPAN class="op">}</SPAN>




    <SPAN class="rem">//--------------------------------------------------------------------</SPAN>
    <SPAN class="kw1">void</SPAN> bezier_arc_svg::init<SPAN class="op">(</SPAN><SPAN class="kw1">double</SPAN> x0<SPAN class="op">,</SPAN> <SPAN class="kw1">double</SPAN> y0<SPAN class="op">,</SPAN> 
                              <SPAN class="kw1">double</SPAN> rx<SPAN class="op">,</SPAN> <SPAN class="kw1">double</SPAN> ry<SPAN class="op">,</SPAN> 
                              <SPAN class="kw1">double</SPAN> angle<SPAN class="op">,</SPAN>
                              <SPAN class="kw1">bool</SPAN> large_arc_flag<SPAN class="op">,</SPAN>
                              <SPAN class="kw1">bool</SPAN> sweep_flag<SPAN class="op">,</SPAN>
                              <SPAN class="kw1">double</SPAN> x2<SPAN class="op">,</SPAN> <SPAN class="kw1">double</SPAN> y2<SPAN class="op">)</SPAN>
    <SPAN class="op">{</SPAN>
        m_radii_ok <SPAN class="op">=</SPAN> <SPAN class="kw1">true</SPAN><SPAN class="op">;</SPAN>

        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>rx <SPAN class="op">&lt;</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> rx <SPAN class="op">=</SPAN> <SPAN class="op">-</SPAN>rx<SPAN class="op">;</SPAN>
        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>ry <SPAN class="op">&lt;</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> ry <SPAN class="op">=</SPAN> <SPAN class="op">-</SPAN>rx<SPAN class="op">;</SPAN>

        <SPAN class="rem">// Calculate the middle point between </SPAN>
        <SPAN class="rem">// the current and the final points</SPAN>
        <SPAN class="rem">//------------------------</SPAN>
        <SPAN class="kw1">double</SPAN> dx2 <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN>x0 <SPAN class="op">-</SPAN> x2<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> dy2 <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN>y0 <SPAN class="op">-</SPAN> y2<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>

        <SPAN class="kw1">double</SPAN> cos_a <SPAN class="op">=</SPAN> cos<SPAN class="op">(</SPAN>angle<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> sin_a <SPAN class="op">=</SPAN> sin<SPAN class="op">(</SPAN>angle<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>

        <SPAN class="rem">// Calculate (x1, y1)</SPAN>
        <SPAN class="rem">//------------------------</SPAN>
        <SPAN class="kw1">double</SPAN> x1 <SPAN class="op">=</SPAN>  cos_a <SPAN class="op">*</SPAN> dx2 <SPAN class="op">+</SPAN> sin_a <SPAN class="op">*</SPAN> dy2<SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> y1 <SPAN class="op">=</SPAN> <SPAN class="op">-</SPAN>sin_a <SPAN class="op">*</SPAN> dx2 <SPAN class="op">+</SPAN> cos_a <SPAN class="op">*</SPAN> dy2<SPAN class="op">;</SPAN>

        <SPAN class="rem">// Ensure radii are large enough</SPAN>
        <SPAN class="rem">//------------------------</SPAN>
        <SPAN class="kw1">double</SPAN> prx <SPAN class="op">=</SPAN> rx <SPAN class="op">*</SPAN> rx<SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> pry <SPAN class="op">=</SPAN> ry <SPAN class="op">*</SPAN> ry<SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> px1 <SPAN class="op">=</SPAN> x1 <SPAN class="op">*</SPAN> x1<SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> py1 <SPAN class="op">=</SPAN> y1 <SPAN class="op">*</SPAN> y1<SPAN class="op">;</SPAN>

        <SPAN class="rem">// Check that radii are large enough</SPAN>
        <SPAN class="rem">//------------------------</SPAN>
        <SPAN class="kw1">double</SPAN> radii_check <SPAN class="op">=</SPAN> px1<SPAN class="op">/</SPAN>prx <SPAN class="op">+</SPAN> py1<SPAN class="op">/</SPAN>pry<SPAN class="op">;</SPAN>
        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>radii_check <SPAN class="op">&gt;</SPAN> <SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> 
        <SPAN class="op">{</SPAN>
            rx <SPAN class="op">=</SPAN> sqrt<SPAN class="op">(</SPAN>radii_check<SPAN class="op">)</SPAN> <SPAN class="op">*</SPAN> rx<SPAN class="op">;</SPAN>
            ry <SPAN class="op">=</SPAN> sqrt<SPAN class="op">(</SPAN>radii_check<SPAN class="op">)</SPAN> <SPAN class="op">*</SPAN> ry<SPAN class="op">;</SPAN>
            prx <SPAN class="op">=</SPAN> rx <SPAN class="op">*</SPAN> rx<SPAN class="op">;</SPAN>
            pry <SPAN class="op">=</SPAN> ry <SPAN class="op">*</SPAN> ry<SPAN class="op">;</SPAN>
            <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>radii_check <SPAN class="op">&gt;</SPAN> <SPAN class="num">10</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> m_radii_ok <SPAN class="op">=</SPAN> <SPAN class="kw1">false</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="op">}</SPAN>

        <SPAN class="rem">// Calculate (cx1, cy1)</SPAN>
        <SPAN class="rem">//------------------------</SPAN>
        <SPAN class="kw1">double</SPAN> sign <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN>large_arc_flag <SPAN class="op">==</SPAN> sweep_flag<SPAN class="op">)</SPAN> ? <SPAN class="op">-</SPAN><SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN> : <SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> sq   <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN>prx<SPAN class="op">*</SPAN>pry <SPAN class="op">-</SPAN> prx<SPAN class="op">*</SPAN>py1 <SPAN class="op">-</SPAN> pry<SPAN class="op">*</SPAN>px1<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> <SPAN class="op">(</SPAN>prx<SPAN class="op">*</SPAN>py1 <SPAN class="op">+</SPAN> pry<SPAN class="op">*</SPAN>px1<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> coef <SPAN class="op">=</SPAN> sign <SPAN class="op">*</SPAN> sqrt<SPAN class="op">(</SPAN><SPAN class="op">(</SPAN>sq <SPAN class="op">&lt;</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> ? <SPAN class="num">0</SPAN> : sq<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> cx1  <SPAN class="op">=</SPAN> coef <SPAN class="op">*</SPAN>  <SPAN class="op">(</SPAN><SPAN class="op">(</SPAN>rx <SPAN class="op">*</SPAN> y1<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> ry<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> cy1  <SPAN class="op">=</SPAN> coef <SPAN class="op">*</SPAN> <SPAN class="op">-</SPAN><SPAN class="op">(</SPAN><SPAN class="op">(</SPAN>ry <SPAN class="op">*</SPAN> x1<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> rx<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>

        <SPAN class="rem">//</SPAN>
        <SPAN class="rem">// Calculate (cx, cy) from (cx1, cy1)</SPAN>
        <SPAN class="rem">//------------------------</SPAN>
        <SPAN class="kw1">double</SPAN> sx2 <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN>x0 <SPAN class="op">+</SPAN> x2<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> sy2 <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN>y0 <SPAN class="op">+</SPAN> y2<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> cx <SPAN class="op">=</SPAN> sx2 <SPAN class="op">+</SPAN> <SPAN class="op">(</SPAN>cos_a <SPAN class="op">*</SPAN> cx1 <SPAN class="op">-</SPAN> sin_a <SPAN class="op">*</SPAN> cy1<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> cy <SPAN class="op">=</SPAN> sy2 <SPAN class="op">+</SPAN> <SPAN class="op">(</SPAN>sin_a <SPAN class="op">*</SPAN> cx1 <SPAN class="op">+</SPAN> cos_a <SPAN class="op">*</SPAN> cy1<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>

        <SPAN class="rem">// Calculate the start_angle (angle1) and the sweep_angle (dangle)</SPAN>
        <SPAN class="rem">//------------------------</SPAN>
        <SPAN class="kw1">double</SPAN> ux <SPAN class="op">=</SPAN>  <SPAN class="op">(</SPAN>x1 <SPAN class="op">-</SPAN> cx1<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> rx<SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> uy <SPAN class="op">=</SPAN>  <SPAN class="op">(</SPAN>y1 <SPAN class="op">-</SPAN> cy1<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> ry<SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> vx <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN><SPAN class="op">-</SPAN>x1 <SPAN class="op">-</SPAN> cx1<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> rx<SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> vy <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN><SPAN class="op">-</SPAN>y1 <SPAN class="op">-</SPAN> cy1<SPAN class="op">)</SPAN> <SPAN class="op">/</SPAN> ry<SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> p<SPAN class="op">,</SPAN> n<SPAN class="op">;</SPAN>

        <SPAN class="rem">// Calculate the angle start</SPAN>
        <SPAN class="rem">//------------------------</SPAN>
        n <SPAN class="op">=</SPAN> sqrt<SPAN class="op">(</SPAN>ux<SPAN class="op">*</SPAN>ux <SPAN class="op">+</SPAN> uy<SPAN class="op">*</SPAN>uy<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        p <SPAN class="op">=</SPAN> ux<SPAN class="op">;</SPAN> <SPAN class="rem">// (1 * ux) + (0 * uy)</SPAN>
        sign <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN>uy <SPAN class="op">&lt;</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> ? <SPAN class="op">-</SPAN><SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN> : <SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> v <SPAN class="op">=</SPAN> p <SPAN class="op">/</SPAN> n<SPAN class="op">;</SPAN>
        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>v <SPAN class="op">&lt;</SPAN> <SPAN class="op">-</SPAN><SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> v <SPAN class="op">=</SPAN> <SPAN class="op">-</SPAN><SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>v <SPAN class="op">&gt;</SPAN>  <SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> v <SPAN class="op">=</SPAN>  <SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> start_angle <SPAN class="op">=</SPAN> sign <SPAN class="op">*</SPAN> acos<SPAN class="op">(</SPAN>v<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>

        <SPAN class="rem">// Calculate the sweep angle</SPAN>
        <SPAN class="rem">//------------------------</SPAN>
        n <SPAN class="op">=</SPAN> sqrt<SPAN class="op">(</SPAN><SPAN class="op">(</SPAN>ux<SPAN class="op">*</SPAN>ux <SPAN class="op">+</SPAN> uy<SPAN class="op">*</SPAN>uy<SPAN class="op">)</SPAN> <SPAN class="op">*</SPAN> <SPAN class="op">(</SPAN>vx<SPAN class="op">*</SPAN>vx <SPAN class="op">+</SPAN> vy<SPAN class="op">*</SPAN>vy<SPAN class="op">)</SPAN><SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        p <SPAN class="op">=</SPAN> ux <SPAN class="op">*</SPAN> vx <SPAN class="op">+</SPAN> uy <SPAN class="op">*</SPAN> vy<SPAN class="op">;</SPAN>
        sign <SPAN class="op">=</SPAN> <SPAN class="op">(</SPAN>ux <SPAN class="op">*</SPAN> vy <SPAN class="op">-</SPAN> uy <SPAN class="op">*</SPAN> vx <SPAN class="op">&lt;</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> ? <SPAN class="op">-</SPAN><SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN> : <SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        v <SPAN class="op">=</SPAN> p <SPAN class="op">/</SPAN> n<SPAN class="op">;</SPAN>
        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>v <SPAN class="op">&lt;</SPAN> <SPAN class="op">-</SPAN><SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> v <SPAN class="op">=</SPAN> <SPAN class="op">-</SPAN><SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>v <SPAN class="op">&gt;</SPAN>  <SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> v <SPAN class="op">=</SPAN>  <SPAN class="num">1</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">double</SPAN> sweep_angle <SPAN class="op">=</SPAN> sign <SPAN class="op">*</SPAN> acos<SPAN class="op">(</SPAN>v<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN><SPAN class="op">!</SPAN>sweep_flag <SPAN class="op">&amp;&amp;</SPAN> sweep_angle <SPAN class="op">&gt;</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> 
        <SPAN class="op">{</SPAN>
            sweep_angle <SPAN class="op">-=</SPAN> <A href="../include/agg_basics.h.html#pi">pi</A> <SPAN class="op">*</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="op">}</SPAN> 
        <SPAN class="kw1">else</SPAN> 
        <SPAN class="kw1">if</SPAN> <SPAN class="op">(</SPAN>sweep_flag <SPAN class="op">&amp;&amp;</SPAN> sweep_angle <SPAN class="op">&lt;</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">)</SPAN> 
        <SPAN class="op">{</SPAN>
            sweep_angle <SPAN class="op">+=</SPAN> <A href="../include/agg_basics.h.html#pi">pi</A> <SPAN class="op">*</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="op">}</SPAN>

        <SPAN class="rem">// We can now build and transform the resulting <A href="../include/agg_arc.h.html#arc">arc</A></SPAN>
        <SPAN class="rem">//------------------------</SPAN>
        m_arc<SPAN class="op">.</SPAN>init<SPAN class="op">(</SPAN><SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">,</SPAN> <SPAN class="num">0</SPAN><SPAN class="op">.</SPAN><SPAN class="num">0</SPAN><SPAN class="op">,</SPAN> rx<SPAN class="op">,</SPAN> ry<SPAN class="op">,</SPAN> start_angle<SPAN class="op">,</SPAN> sweep_angle<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <A href="../include/agg_trans_affine.h.html#trans_affine">trans_affine</A> mtx <SPAN class="op">=</SPAN> <A href="../include/agg_trans_affine.h.html#trans_affine_rotation">trans_affine_rotation</A><SPAN class="op">(</SPAN>angle<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        mtx <SPAN class="op">*=</SPAN> <A href="../include/agg_trans_affine.h.html#trans_affine_translation">trans_affine_translation</A><SPAN class="op">(</SPAN>cx<SPAN class="op">,</SPAN> cy<SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        
        <SPAN class="kw1">for</SPAN><SPAN class="op">(</SPAN><SPAN class="kw1">unsigned</SPAN> i <SPAN class="op">=</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">;</SPAN> i <SPAN class="op">&lt;</SPAN> m_arc<SPAN class="op">.</SPAN>num_vertices<SPAN class="op">(</SPAN><SPAN class="op">)</SPAN><SPAN class="op">-</SPAN><SPAN class="num">2</SPAN><SPAN class="op">;</SPAN> i <SPAN class="op">+=</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">)</SPAN>
        <SPAN class="op">{</SPAN>
            mtx<SPAN class="op">.</SPAN>transform<SPAN class="op">(</SPAN>m_arc<SPAN class="op">.</SPAN>vertices<SPAN class="op">(</SPAN><SPAN class="op">)</SPAN> <SPAN class="op">+</SPAN> i<SPAN class="op">,</SPAN> m_arc<SPAN class="op">.</SPAN>vertices<SPAN class="op">(</SPAN><SPAN class="op">)</SPAN> <SPAN class="op">+</SPAN> i <SPAN class="op">+</SPAN> <SPAN class="num">1</SPAN><SPAN class="op">)</SPAN><SPAN class="op">;</SPAN>
        <SPAN class="op">}</SPAN>

        <SPAN class="rem">// We must make sure that the starting and ending points</SPAN>
        <SPAN class="rem">// exactly coincide with the initial (x0,y0) and (x2,y2)</SPAN>
        m_arc<SPAN class="op">.</SPAN>vertices<SPAN class="op">(</SPAN><SPAN class="op">)</SPAN><SPAN class="op">[</SPAN><SPAN class="num">0</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> x0<SPAN class="op">;</SPAN>
        m_arc<SPAN class="op">.</SPAN>vertices<SPAN class="op">(</SPAN><SPAN class="op">)</SPAN><SPAN class="op">[</SPAN><SPAN class="num">1</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> y0<SPAN class="op">;</SPAN>
        <SPAN class="kw1">if</SPAN><SPAN class="op">(</SPAN>m_arc<SPAN class="op">.</SPAN>num_vertices<SPAN class="op">(</SPAN><SPAN class="op">)</SPAN> <SPAN class="op">&gt;</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">)</SPAN>
        <SPAN class="op">{</SPAN>
            m_arc<SPAN class="op">.</SPAN>vertices<SPAN class="op">(</SPAN><SPAN class="op">)</SPAN><SPAN class="op">[</SPAN>m_arc<SPAN class="op">.</SPAN>num_vertices<SPAN class="op">(</SPAN><SPAN class="op">)</SPAN> <SPAN class="op">-</SPAN> <SPAN class="num">2</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> x2<SPAN class="op">;</SPAN>
            m_arc<SPAN class="op">.</SPAN>vertices<SPAN class="op">(</SPAN><SPAN class="op">)</SPAN><SPAN class="op">[</SPAN>m_arc<SPAN class="op">.</SPAN>num_vertices<SPAN class="op">(</SPAN><SPAN class="op">)</SPAN> <SPAN class="op">-</SPAN> <SPAN class="num">1</SPAN><SPAN class="op">]</SPAN> <SPAN class="op">=</SPAN> y2<SPAN class="op">;</SPAN>
        <SPAN class="op">}</SPAN>
    <SPAN class="op">}</SPAN>


<SPAN class="op">}</SPAN>
</PRE></TD></TR></TABLE><FONT style="margin-left:1em"><I></I></FONT><TABLE width="640px" bgcolor="#583927" height="1px" border="0" cellspacing="0" cellpadding="0" style="margin:0px;"><TR><TD></TD></TR></TABLE>
<TABLE width="640px" border="0" cellspacing="0" cellpadding="0">
<TR><TD><CENTER><SPAN class="authors">
Copyright <SPAN class="larger">&#169;</SPAN> 2002-2006
<A href="../../mcseem/index.html"><B>Maxim Shemanarev</B></A>
</SPAN></CENTER></TD></TR>
<TR><TD><CENTER><SPAN class="authors">
Web Design and Programming
<A href="../../mcseem/index.html"><B>Maxim Shemanarev</B></A>
</SPAN></CENTER></TD></TR>
</TABLE>
<BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/>
<BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/>
<BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/>
<BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/><BR/>
</HTML>
